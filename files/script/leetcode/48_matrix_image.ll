
import lib/basic_lib
require array_loop

def rotate_matrix_image()
{
    var l1[4] = [5, 1, 9, 11]
    var l2[4] = [2, 4, 8, 10]
    var l3[4] = [13, 3, 6, 7]
    var l4[4] = [15, 14, 12, 16]
    var matrix<> = <1:l1, 2:l2, 3:l3, 4:l4>
    var N = 4
    var i
    var j

    for (i = 1; i <= N/2; i+=1){
        var line1
        var line2
        var tmp_hash<>

        line1 = matrix.find_item(i)
        line2 = matrix.find_item(N - i + 1)

        tmp_hash.add_key_item(N - i + 1, line1)
        tmp_hash.add_key_item(i, line2)

        line1 = tmp_hash.find_item(N - i + 1)
        line2 = tmp_hash.find_item(i)

        matrix.add_key_item(N -i + 1, line1)
        matrix.add_key_item(i, line2)
    }

    for (i = 1; i <= N; i+=1){
        var line1
        line1 = matrix.find_item(i)

        for (j = i; j <= N; j+=1){
            var line2
            var tmp

            if (i == j){
                continue
            }
            line2 = matrix.find_item(j)

            os.print("swap", line1[j-1], line2[i-1])

            tmp = line1[j-1]
            line1[j-1] = line2[i-1]
            line2[i-1] = tmp
        }
    }

    for (i = 1; i <= N; i+=1){
       var line1

       line1 = matrix.find_item(i)
       array_loop(line1)
    }

}

def rotate_matrix_image2()
{
    var l1[4] = [5, 1, 9, 11]
    var l2[4] = [2, 4, 8, 10]
    var l3[4] = [13, 3, 6, 7]
    var l4[4] = [15, 14, 12, 16]
    var matrix<> = <1:l1, 2:l2, 3:l3, 4:l4>
    var N = 4
    var i
    var j

    for (i = 1; i <= N/2; i+=1){
        for (j = 0; j < N; j+=1){
            var tmp

            tmp = matrix{i}[j]
            matrix{i}[j] = matrix{N - i + 1}[j]
            matrix{N - i + 1}[j] = tmp
        }
    }

    for (i = 1; i <= N; i+=1){
        for (j = i; j <= N; j+=1){
            var tmp

            if (i == j){
                continue
            }

            os.print("swap", matrix{i}[j-1], matrix{j}[i-1])

            tmp = matrix{i}[j-1]
            matrix{i}[j-1] = matrix{j}[i-1]
            matrix{j}[i-1] = tmp
        }
    }

    for (i = 1; i <= N; i+=1){
       array_loop(matrix{i})
    }
}

def main()
{
#    rotate_matrix_image()
    rotate_matrix_image2()
}
#script("script/leetcode/48_matrix_image.ll")
