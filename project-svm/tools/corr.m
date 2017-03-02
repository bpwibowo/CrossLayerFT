%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Correlation coefficient analysis
%%
% Author:        Bagus Wibowo
% First created: 3/11/2016

function corr(id, matrix, outfile)

% get the correlation coefficient of each parameters
R = corrcoef(matrix);
R2 = R.^2; % get R squared value

fout = fopen(outfile,'a');
fprintf(fout, '%s', id);
fprintf(fout, '\t%f', R2(1,2:end));
fprintf(fout, '\n');
fclose(fout);
