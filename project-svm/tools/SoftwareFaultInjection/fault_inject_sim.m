clear;
result_diff_max_arr = [];
result_diff_max_arr_1 = [];
result_diff_max_arr_2 = [];
result_diff_max_arr_3 = [];
result_diff_max_arr_4 = [];
result_diff_max_arr_5 = [];
K_series_arr = [];

for loop = 0:11
    bit_flipped_count = 0;
    bit_flipped = 0;
    fault_insert = 0;
    result_diff_count = [];
    result_diff_count_1 = [];
    result_diff_count_2 = [];
    result_diff_count_3 = [];
    result_diff_count_4 = [];
    result_diff_count_5 = [];
    measure_diff_count = [];
    fault_insert = 0;
    kalman_ex ();
    fault_insert = 1;
    for i = 1:2000
        kalman_ex ();
        bit_flipped_count = bit_flipped_count + bit_flipped;
        result_diff_count = [result_diff_count abs(result_diff_arr)];
        result_diff_count_1 = [result_diff_count_1 abs(result_diff_arr_1)];
        result_diff_count_2 = [result_diff_count_2 abs(result_diff_arr_2)];
        result_diff_count_3 = [result_diff_count_3 abs(result_diff_arr_3)];
        result_diff_count_4 = [result_diff_count_4 abs(result_diff_arr_4)];
        result_diff_count_5 = [result_diff_count_5 abs(result_diff_arr_5)];
        measure_diff_count = [measure_diff_count abs(measurement_diff_arr)];
    end
    result_diff_max_arr = [result_diff_max_arr 100*max(result_diff_count)];
    result_diff_max_arr_1 = [result_diff_max_arr_1 100*max(result_diff_count_1)];
    result_diff_max_arr_2 = [result_diff_max_arr_2 100*max(result_diff_count_2)];
    result_diff_max_arr_3 = [result_diff_max_arr_3 100*max(result_diff_count_3)];
    result_diff_max_arr_4 = [result_diff_max_arr_4 100*max(result_diff_count_4)];
    result_diff_max_arr_5 = [result_diff_max_arr_5 100*max(result_diff_count_5)];
    K_series_arr = [K_series_arr K(1)];
    K(1)
end

result_diff_max_arr
result_diff_max_arr_1
result_diff_max_arr_2
result_diff_max_arr_3
result_diff_max_arr_4
result_diff_max_arr_5
K_series_arr
