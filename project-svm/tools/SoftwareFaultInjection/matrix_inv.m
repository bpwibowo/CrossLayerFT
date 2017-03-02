function z = matrix_inv(x)
    matrix_dim_x = size(x);
    n = matrix_dim_x(1);
    if matrix_dim_x(1) == matrix_dim_x(2)
        Identity_matrix = eye(matrix_dim_x(1));
        Identity_matrix = [Identity_matrix';sum(Identity_matrix')]';
        for i = 1:n
            if (x(i,i) == 0)
                for j = (i+1):n
                    if (x(j,i) ~= 0)
                        x([i j],:) = x([j i], :);
                        Identity_matrix([i j],:) = Identity_matrix([j i], :);
                        break
                    end
                end
            end
            if (x(i,i) == 0)
                fprintf('Inverse does not exist\n');
                return
            end
            scale = x(i,i);
            x(i,:) = x(i,:)/scale;
            Identity_matrix(i,:) = Identity_matrix(i,:)/scale;
            if (i < n)
                for j = (i+1):n
                    factor = x(j,i);
                    x(j,:) = x(j,:) - factor * x(i,:);
                    Identity_matrix(j,:) = Identity_matrix(j,:) - factor * Identity_matrix(i,:);
                end
            end            
        end
        for col = n:-1:2
            for row = (col-1):-1:1
                factor = x(row,col);
                x(row,:) = x(row,:) - factor * x(col,:);
                Identity_matrix(row,:) = Identity_matrix(row,:) - factor * Identity_matrix(col,:);
            end
        end
        temp=Identity_matrix
        z=Identity_matrix(1:n,1:n);
    else
        fprintf('Row(%d) not equal to column(%d) in inversion\n', matrix_dim_x(1), matrix_dim_x(2));
        return
    end
end
