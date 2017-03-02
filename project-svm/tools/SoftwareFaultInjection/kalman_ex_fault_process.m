% written by StudentDave 
%for licensing and usage questions 
%email scienceguy5000 at gmail. com

%Bayesian Ninja tracking Quail using kalman filter

%clear all
bit_flipped = 0;
result_diff_arr = [];
result_diff_arr_1 = [];
result_diff_arr_2 = [];
result_diff_arr_3 = [];
result_diff_arr_4 = [];
result_diff_arr_5 = [];

measurement_diff_arr = [];
%% define our meta-variables (i.e. how long and often we will sample)
duration = 20;  %how long the Quail flies
dt = .1;  %The Ninja continuously looks for the birdy,
if (fault_insert == 1)
    bit_flip_time = randi(duration*10);
else
    bit_flip_time = duration*20;
end
%but we'll assume he's just repeatedly sampling over time at a fixed interval

%% Define update equations (Coefficent matrices): A physics based model for where we expect the Quail to be [state transition (state + velocity)] + [input control (acceleration)]
A = [1 dt; 0 1] ; % state transition matrix:  expected flight of the Quail (state prediction)
B = [dt^2/2; dt]; %input control matrix:  expected effect of the input accceleration on the state.
C = [1 0]; % measurement matrix: the expected measurement given the predicted state (likelihood)
%since we are only measuring position (too hard for the ninja to calculate speed), we set the velocity variable to
%zero.

%% define main variables
u = 1.25; % define acceleration magnitude
Q= [0; 0]; %initized state--it has two components: [position; velocity] of the Quail
Q_estimate = Q;  %x_estimate of initial location estimation of where the Quail is (what we are updating)
QuailAccel_noise_mag = 1.090 - loop*0.05; %process noise: the variability in how fast the Quail is speeding up (stdv of acceleration: meters/sec^2)
NinjaVision_noise_mag = 0.001 + loop*0.0015;  %measurement noise: How mask-blinded is the Ninja (stdv of location, in meters)
Ez = NinjaVision_noise_mag^2;% Ez convert the measurement noise (stdv) into covariance matrix
Ex = QuailAccel_noise_mag^2 * [dt^4/4 0; 0 dt^2]; % Ex convert the process noise (stdv) into covariance matrix
P = Ex; % estimate of initial Quail position variance (covariance matrix)

%% initize result variables
% Initialize for speed
if (fault_insert == 0)
    Q_loc = []; % ACTUAL Quail flight path
    vel = []; % ACTUAL Quail velocity
    Q_loc_meas = []; % Quail path that the Ninja sees
end

%% simulate what the Ninja sees over time
%figure(2);clf
%figure(1);clf
if (fault_insert == 0)
    for t = 0 : dt: duration

        % Generate the Quail flight
        QuailAccel_noise = QuailAccel_noise_mag * [(dt^2/2)*randn; dt*randn];
        Q= A * Q+ B * u + QuailAccel_noise;
        % Generate what the Ninja sees
        NinjaVision_noise = NinjaVision_noise_mag * randn;
        y = C * Q+ NinjaVision_noise;
        Q_loc = [Q_loc; Q(1)];
        Q_loc_meas = [Q_loc_meas; y];
        vel = [vel; Q(2)];
        %iteratively plot what the ninja sees
        %plot(0:dt:t, Q_loc, '-r.')
        %plot(0:dt:t, Q_loc_meas, '-k.')
        %axis([0 duration -30 400])
        %hold on
    end
end

%plot theoretical path of ninja that doesn't use kalman
%plot(0:dt:t, smooth(Q_loc_meas), '-g.')

%plot velocity, just to show that it's constantly increasing, due to
%constant acceleration
%figure(2);
%plot(0:dt:t, vel, '-b.')


%% Do kalman filtering
%initize estimation variables
Q_loc_estimate = []; %  Quail position estimate
vel_estimate = []; % Quail velocity estimate
Q= [0; 0]; % re-initized state
P_estimate = P;
P_mag_estimate = [];
predic_state = [];
predic_var = [];
K_arr = [];
%figure(2);
for t = 1:length(Q_loc)
    % Predict next state of the quail with the last state and predicted motion.
    Q_estimate = A * Q_estimate + B * u;
    predic_state = [predic_state; Q_estimate(1)] ;
    %predict next covariance
    P = A * P * A' + Ex;
    predic_var = [predic_var; P] ;
    % predicted Ninja measurement covariance
    % Kalman Gain
    K = P*C'*inv(C*P*C'+Ez); %K_2 = K(2); K(2) = 0;
    % Update the state estimate.
    if t == bit_flip_time
        Q_estimate_no_fualt = Q_estimate(1);
        Q_estimate(1) = bit_flip_random(Q_estimate(1), 256.00, 16, 1, 'Q_estimate(1)');
        measurement_diff = abs(Q_estimate(1) - Q_estimate_no_fualt)/Q_estimate_no_fualt;
        measurement_diff_arr = [measurement_diff_arr measurement_diff];
        %if (abs(Q_loc_meas_fault - Q_loc_meas(t)) > 0.002)
            %plot(t/10,Q_loc_meas_fault,'*');
            %axis([0 duration -30 400])
            %hold on
            bit_flipped = bit_flipped + 1;
        %end
        Q_estimate = Q_estimate + K * (Q_loc_meas(t) - C * Q_estimate);
        %fprintf('Q_estimate(1): %f => %f\n', Q_estimate_no_fault(1), Q_estimate(1));
        %fprintf('Q_estimate(2): %f => %f\n', Q_estimate_no_fault(2), Q_estimate(2));
        result_diff = abs(Q_estimate(1) - Q_loc_estimate_no_fault(t))/256;
        result_diff_arr = [result_diff_arr result_diff];
    else
        Q_estimate = Q_estimate + K * (Q_loc_meas(t) - C * Q_estimate);
    end

    % update covariance estimation.
    %K(2) = K_2;
    P =  (eye(2)-K*C)*P;
    %Store for plotting
    Q_loc_estimate = [Q_loc_estimate; Q_estimate(1)];
    vel_estimate = [vel_estimate; Q_estimate(2)];
    P_mag_estimate = [P_mag_estimate; P(1)];
    K_arr = [K_arr, K];
    if (t == (bit_flip_time+1))
        result_diff = abs(Q_estimate(1) - Q_loc_estimate_no_fault(t))/256;
        result_diff_arr_1 = [result_diff_arr_1 result_diff];
    end
    if (t == (bit_flip_time+2))
        result_diff = abs(Q_estimate(1) - Q_loc_estimate_no_fault(t))/256;
        result_diff_arr_2 = [result_diff_arr_2 result_diff];
    end
    if (t == (bit_flip_time+4))
        result_diff = abs(Q_estimate(1) - Q_loc_estimate_no_fault(t))/256;
        result_diff_arr_3 = [result_diff_arr_3 result_diff];
    end
    if (t == (bit_flip_time+8))
        result_diff = abs(Q_estimate(1) - Q_loc_estimate_no_fault(t))/256;
        result_diff_arr_4 = [result_diff_arr_4 result_diff];
    end
    if (t == (bit_flip_time+16))
        result_diff = abs(Q_estimate(1) - Q_loc_estimate_no_fault(t))/256;
        result_diff_arr_5 = [result_diff_arr_5 result_diff];
    end
    %pause(0.1)
end

if (fault_insert == 0)
    Q_loc_estimate_no_fault = Q_loc_estimate;
end

% Plot the results
%figure(2);
%tt = 0 : dt : duration;
%plot(tt,Q_loc,'-r.',tt,Q_loc_meas,'-k.', tt,Q_loc_estimate,'-g.');
%axis([0 duration -30 400])


%plot the evolution of the distributions
%{
figure(3);clf
for T = 1:length(Q_loc_estimate)
clf
    x = Q_loc_estimate(T)-5:.01:Q_loc_estimate(T)+5; % range on x axis
      
    %predicted next position of the quail     
    hold on
    mu = predic_state(T); % mean
    sigma = predic_var(T); % standard deviation
    y = normpdf(x,mu,sigma); % pdf
    y = y/(max(y));
    hl = line(x,y,'Color','m'); % or use hold on and normal plot
       
    %data measured by the ninja
    mu = Q_loc_meas(T); % mean
    sigma = NinjaVision_noise_mag; % standard deviation
    y = normpdf(x,mu,sigma); % pdf
    y = y/(max(y));
    hl = line(x,y,'Color','k'); % or use hold on and normal plot
    
    %combined position estimate
    mu = Q_loc_estimate(T); % mean
    sigma = P_mag_estimate(T); % standard deviation
    y = normpdf(x,mu,sigma); % pdf
    y = y/(max(y));
    hl = line(x,y, 'Color','g'); % or use hold on and normal plot
    axis([Q_loc_estimate(T)-5 Q_loc_estimate(T)+5 0 1]);     

    
    %actual position of the quail
    plot(Q_loc(T)); 
    ylim=get(gca,'ylim');
    line([Q_loc(T);Q_loc(T)],ylim.','linewidth',2,'color','b');
    legend('state predicted','measurement','state estimate','actual Quail position')
end
%}