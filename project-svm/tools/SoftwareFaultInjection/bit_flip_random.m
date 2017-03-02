function y = bit_flip_random(x, fp_factor, max_bit_flip, bit_flip_prob, var_name)
    original_value = x;
    y = x;
    if (randi(bit_flip_prob) == 1 )
        bit_flip_pos = int64(bitshift(1, randi(max_bit_flip)));
        fixed_point_value = int64(original_value*fp_factor);
        fixed_point_value_xored = bitxor(fixed_point_value, bit_flip_pos);
        fixed_point_value_xored = double(double(fixed_point_value_xored)/fp_factor);
        y = fixed_point_value_xored;
        %fprintf('%s: %f => %f\n', var_name, x, y);
    end

end
