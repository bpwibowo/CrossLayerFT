%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% AVF-R regression and correlation analysis
%%
% Author:        Bagus Wibowo
% First created: 3/2/2016

function avfr(infile, outfile, excluded_param_idx, target_r2, min_params, max_params)

% disable regression rank deficient warning
warning('off','stats:regress:RankDefDesignMat');

A = textread(infile);

% initialize y, params, and indices
y = A(:,1);
P = A(:,2:end);
indices = [1:size(A,2)-1];

for idx = sort(excluded_param_idx,'descend')
	fprintf('exclude %d\n', idx)
	indices(idx) = [];
	A(:,idx+1) = [];
end

% get the correlation coefficient of each parameters
R_init = corrcoef(A);
R_init = R_init(2:end,1); % get a vector of parameters' R
R_init(isnan(R_init)) = 0; % replace NaN with zero
R_init = R_init.^2; % get R squared value

% get index of the highest correlated parameter
[M,I] = max(R_init);

fprintf('Highest parameter index: %d\n', indices(I));
fprintf('Highest parameter correlation: %f\n\n', M);

% extract the first parameter
params = P(:,indices(I));
params_idx = [indices(I)];
indices(I) = []; % remove I from indices

[B,bint,r,rint,stats] = regression(y,params);
R = stats(1);

iter = 1;

while (size(params_idx,2) < min_params) | (size(params_idx,2) < max_params & R < target_r2)

	fprintf('iteration #%d: ', iter);
	fprintf('R so far is %f', R);
	fprintf(', params_idx:');
	fprintf(' %d', params_idx);
	fprintf('\n');

	% now try adding new parameter, do regression analysis, and find
	% the most correlated new parameter
	R = [];

	for index = indices
		p = [params P(:,index)];
		[B,bint,r,rint,stats] = regression(y,p);
		R = [R stats(1)];
		if R < 0
			fprintf('R=%f\n', R);
		end
	end

	[R,I] = max(R);

	params = [params P(:,indices(I))];
	params_idx = [params_idx indices(I)];
	indices(I) = []; % remove I from indices

	iter=iter+1;

end

if iter > 1
	fprintf('iteration #%d: ', iter);
	fprintf('R so far is %f', R);
	fprintf(', params_idx:');
	fprintf(' %d', params_idx);
	fprintf('\n');
end

[B,bint,r,rint,stats] = regression(y,params);
R = stats(1);

fout = fopen(outfile,'w');
fprintf(fout,'FinalR: %f\n', R);
fprintf(fout,'FinalB:\n');
fprintf(fout,'- %1.30e\n', B);
fprintf(fout,'params_idx:\n');
fprintf(fout,'- %d\n', params_idx);
