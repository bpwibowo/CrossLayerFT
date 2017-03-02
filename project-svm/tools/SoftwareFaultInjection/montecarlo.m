x=0; y=0; val=0; error=1; sampled=0; hit=0; i=0;
tolerance = 0.001;

result_diff_arr = [];
bit_flipped = 0;
bit_flip_time = 1000000000000; %randi(3);
bit_flip_done = 0;
loop_count = 0;
while((error>tolerance) || ~hit)
    for i = 1 : 1000000
        x = rand;
        %x = bit_flip_random(x, 1024.00, 10, 100000, 'x');
        y = rand;
        %y = bit_flip_random(y, 1024.00, 10, 100000, 'y');
        x2 = x*x;
        %x2 = bit_flip_random(x2, 1024.00, 10, 100000, 'x2');
        y2 = y*y;
        %y2 = bit_flip_random(y2, 1024.00, 10, 100000, 'y2');
        dist = (x2 + y2);
        %dist = bit_flip_random(dist, 1024.00, 10, 100000, 'dist');
        if ( dist < 1)
            hit = hit+1;
        end
        sampled = sampled + 1;
    %sampled = bit_flip_random(sampled, 1.00, 20, 1000000, 'sampled');
    %hit = bit_flip_random(hit, 1.00, 20, 1000000, 'hit');
    end
    loop_count = loop_count + 1;
    val = hit/sampled;
    error = sqrt(val*(1-val)/sampled)*4;
    val = val*4;
    if ( (bit_flip_time == loop_count) && (bit_flip_done == 0) )
        val = bit_flip_random(val, 1024.0, 12, 1, 'val');
        bit_flipped = bit_flipped + 1;
        bit_flip_done = 1;
    end
    %fprintf('Pi = %f +/- %5.3e at %ldM samples\n', val, error, sampled/1000000);
end
result_diff = val - 3.141592;
result_diff_arr = [result_diff_arr result_diff];
fprintf('Pi = %f\n', val);