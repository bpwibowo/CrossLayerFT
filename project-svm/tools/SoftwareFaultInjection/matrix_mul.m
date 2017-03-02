function z = matrix_mul(x, y)
    matrix_dim_x = size(x)
    matrix_dim_y = size(y)
    rows = matrix_dim_x(1)
    columns = matrix_dim_x(2)
    x_col_csum = [x;sum(x)]
    y_row_csum = [y';sum(y')]'
    z_csum = x_col_csum * y_row_csum

    z_correct = z_csum(1:rows,1:columns)
    z_raw = z_correct
    row_rand = randi(rows)
    column_rand = randi(columns)
    z_raw(row_rand, column_rand) = 0
    
    z_col_csum = [z_raw;sum(z_raw)]
    z_comp_csum = [z_col_csum';sum(z_col_csum')]'
    if ~isequal(z_comp_csum, z_csum)
        compare_matrix = (z_comp_csum == z_csum)
        for entry = 1:(rows)
            if ~compare_matrix(entry,(columns+1))
                buggy_row = entry
                break
            end
        end
        for entry = 1:(columns)
            if ~compare_matrix((rows+1),entry)
                buggy_column = entry
                break
            end
        end
        corrected_entry = z_csum(buggy_row, (columns+1));
        for entry = 1:(columns)
            if (entry ~= buggy_column)
                corrected_entry = corrected_entry - z_csum(buggy_row,entry);
            end
        end
        corrected_entry
        z_raw(buggy_row, buggy_column) = corrected_entry
        z = z_raw
    else
        z = z_raw;
    end
end
