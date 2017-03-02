bit_flipped_count = 0;
bit_flipped = 0;
result_diff_count = [];
%measure_diff_count = [];
for i = 1:2000
    montecarlo();
    bit_flipped_count = bit_flipped_count + bit_flipped;
    result_diff_count = [result_diff_count result_diff_arr];
    %measure_diff_count = [measure_diff_count measurement_diff_arr];
end