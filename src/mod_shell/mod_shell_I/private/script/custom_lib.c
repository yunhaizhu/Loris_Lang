/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */
     
/**
 * @file    shell_lib.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2022-02-14
 *
 */
//
// Created by yun on 12/24/21.
//

#include "std_common.h"
#include "tiny-json.h"
#include "virtual_machine.h"
#include "virtual_machine_library.h"
#include "virtual_machine_safe_var.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * library_save_file
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_save_file(environment_vm_t *vm, IN std_int_t args)
{
    own_value_t file_name;
    own_value_t file_body;
    std_char_t *string_file_name;
    std_char_t *string_file_body;
    FILE *fp = NULL;

    file_body = Pop(vm);
    file_body = get_VAR(file_body, NAN_BOX_Null, STD_BOOL_FALSE);
    file_name = Pop(vm);
    file_name = get_VAR(file_name, NAN_BOX_Null, STD_BOOL_FALSE);

    string_file_name = get_own_value_object_string(file_name);
    string_file_body = get_own_value_object_string(file_body);

    fp = fopen(string_file_name, "w+");
    if (fp == NULL) {
        STD_LOG(ERR, "open file %s failed\n", string_file_name);
        return;
    }
    fprintf(fp, "%s", string_file_body);
    fclose(fp);
}


/**
 * library_diff
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_diff(environment_vm_t *vm, IN std_int_t args)
{
    own_value_t file_x;
    own_value_t file_y;
    std_char_t cmd[BUF_SIZE_128] = {0};
    std_char_t *string_file_x;
    std_char_t *string_file_y;

    file_y = Pop(vm);
    file_y = get_VAR(file_y, NAN_BOX_Null, STD_BOOL_FALSE);
    file_x = Pop(vm);
    file_x = get_VAR(file_x, NAN_BOX_Null, STD_BOOL_FALSE);

    string_file_x = get_own_value_object_string(file_x);
    string_file_y = get_own_value_object_string(file_y);
    snprintf(cmd,sizeof(cmd), "diff %s %s |colordiff", string_file_x, string_file_y );

    std_int_t ret = system(cmd);
    if (ret != 0) {
        STD_LOG(ERR, "diff file %s %s failed\n", string_file_x, string_file_y);
        return;
    }
}


/* Function to make HTTP POST request to OpenAI GPT API */
int make_request(char *API_KEY, char *API_ENDPOINT, char *prompt, char *model, char *message, float temperature, char **response)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    char *post_fields;
    cJSON *root, *messages, *message_obj;

    /* Create JSON request payload */
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "model", model);
    messages = cJSON_AddArrayToObject(root, "messages");
    message_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(message_obj, "role", "user");
    cJSON_AddStringToObject(message_obj, "content", message);
    cJSON_AddItemToArray(messages, message_obj);

    if (prompt != NULL && std_safe_strlen(prompt, 8192) > 0) {
        message_obj = cJSON_CreateObject();
        cJSON_AddStringToObject(message_obj, "role", "system");
        cJSON_AddStringToObject(message_obj, "content", prompt);
        cJSON_AddItemToArray(messages, message_obj);
    }

    cJSON_AddNumberToObject(root, "temperature", temperature);

    /* Convert JSON payload to string */
    post_fields = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    /* Set HTTP request headers */
    headers = curl_slist_append(headers, "Content-Type: application/json");
    char auth_header[128];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", API_KEY);
    headers = curl_slist_append(headers, auth_header);

    /* Initialize CURL */
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2); // Compliant; enables TLSv1.2 / TLSv1.3 version only
    if (!curl) {
        fprintf(stderr, "Error initializing CURL.\n");
        return 1;
    }

    /* Set CURL options */
    curl_easy_setopt(curl, CURLOPT_URL, API_ENDPOINT);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);


    /* Make HTTP request */
    res = curl_easy_perform(curl);

    /* Cleanup */
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(post_fields);
    return (int) res;
}


/**
 * library_ask_gpt
 * @brief   
 * @param   vm
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_ask_gpt(environment_vm_t *vm, IN std_int_t args)
{
    char *response = NULL;
    int res;
    own_value_t ret_obj;
    own_value_t model_obj;
    own_value_t ask_text_obj;
    own_value_t prompt_obj;
    own_value_t api_endpoint_obj;
    own_value_t api_key_obj;
    std_char_t *model = NULL;
    std_char_t *ask_text = NULL;
    std_char_t *prompt = NULL;
    std_char_t *api_endpoint = NULL;
    std_char_t *api_key = NULL;
    own_value_t response_obj;

    ret_obj = Pop(vm);
    ask_text_obj = Pop(vm);
    model_obj = Pop(vm);
    prompt_obj = Pop(vm);
    api_endpoint_obj = Pop(vm);
    api_key_obj = Pop(vm);

    model_obj = get_VAR(model_obj, NAN_BOX_Null, STD_BOOL_FALSE);
    ask_text_obj = get_VAR(ask_text_obj, NAN_BOX_Null, STD_BOOL_FALSE);
    prompt_obj = get_VAR(prompt_obj, NAN_BOX_Null, STD_BOOL_FALSE);
    api_endpoint_obj = get_VAR(api_endpoint_obj, NAN_BOX_Null, STD_BOOL_FALSE);
    api_key_obj = get_VAR(api_key_obj, NAN_BOX_Null, STD_BOOL_FALSE);

    model = get_own_value_object_string(model_obj);
    ask_text = get_own_value_object_string(ask_text_obj);
    prompt = get_own_value_object_string(prompt_obj);
    api_endpoint = get_own_value_object_string(api_endpoint_obj);
    api_key = get_own_value_object_string(api_key_obj);

    res = make_request(api_key, api_endpoint, prompt, model, ask_text, 0.7, &response);
    if (res == CURLE_OK) {
        cJSON *root, *choices, *choice, *message;
        root = cJSON_Parse(response);
        choices = cJSON_GetObjectItem(root, "choices");
        choice = cJSON_GetArrayItem(choices, 0);
        message = cJSON_GetObjectItem(choice, "message");

        response_obj = make_own_value_object_string(cJSON_GetObjectItem(message, "content")->valuestring);
        cJSON_Delete(root);
    } else {
        fprintf(stderr, "Error making request: %s\n", curl_easy_strerror(res));
        response_obj = make_own_value_object_string(curl_easy_strerror(res));
    }

    set_VAR(ret_obj, NAN_BOX_Null, response_obj);

    free(response);
}


/**
 * library_init
 * @brief   
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_init(environment_vm_t *vm, std_int_t *register_id)
{
    library_func_register(vm,register_id, "function__diff", 2, library_diff);
    library_func_register(vm,register_id, "function__ask_gpt", 6, library_ask_gpt);
    library_func_register(vm,register_id, "function__save_file", 2, library_save_file);

}