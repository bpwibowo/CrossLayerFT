% written by StudentDave
%for licensing and usage questions
%email scienceguy5000 at gmail. com

%Bayesian Ninja tracking Quail using kalman filter

clear all
%% define our meta-variables (i.e. how long and often we will sample)
duration = 30  %how long the Quail flies
dt = .1;  %The Ninja continuously looks for the birdy,
%but we'll assume he's just repeatedly sampling over time at a fixed interval

%% Define update equations (Coefficent matrices): A physics based model for where we expect the Quail to be [state transition (state + velocity)] + [input control (acceleration)]
A = [1 dt; 0 1] ; % state transition matrix:  expected flight of the Quail (state prediction)
B = [dt^2/2; dt]; %input control matrix:  expected effect of the input accceleration on the state.
C = [1 0]; % measurement matrix: the expected measurement given the predicted state (likelihood)
%since we are only measuring position (too hard for the ninja to calculate speed), we set the velocity variable to
%zero.

%% define main variables
u = 1.25; % define acceleration magnitude
Q= [0; 2]; %initized state--it has two components: [position; velocity] of the Quail
Q_no_noise= [0; 2]; %initized state--it has two components: [position; velocity] of the Quail; used as no noise case
Q_estimate = Q;  %x_estimate of initial location estimation of where the Quail is (what we are updating)
QuailAccel_noise_mag = 0.05; %process noise: the variability in how fast the Quail is speeding up (stdv of acceleration: meters/sec^2)
NinjaVision_noise_mag = 10;  %measurement noise: How mask-blinded is the Ninja (stdv of location, in meters)
Ez = NinjaVision_noise_mag^2;% Ez convert the measurement noise (stdv) into covariance matrix
Ex = (QuailAccel_noise_mag)^2 * [dt^4/4 0; 0 dt^2]; % Ex convert the process noise (stdv) into covariance matrix
P = Ex; % estimate of initial Quail position variance (covariance matrix)

%% initize result variables
% Initialize for speed
Q_loc_no_noise = []; % ACTUAL Quail flight path
Q_loc = []; % ACTUAL Quail flight path
vel = []; % ACTUAL Quail velocity
Q_loc_meas = []; % Quail path that the Ninja sees


%% simulate what the Ninja sees over time
figure(2);clf
figure(1);clf
for t = 0 : dt: duration

    % Generate the Quail flight
    QuailAccel_noise = QuailAccel_noise_mag * [(dt^2/2)*randn; dt*randn];
    Q= A * Q+ B * u + QuailAccel_noise;
    Q_no_noise= A * Q_no_noise+ B * u;
    % Generate what the Ninja sees
    NinjaVision_noise = NinjaVision_noise_mag * randn;
    y = C * Q+ NinjaVision_noise;
    Q_loc = [Q_loc; Q(1)];
    Q_loc_no_noise = [Q_loc_no_noise; Q_no_noise(1)];
    if(t > 11.1 && t < 11.3)
        Q_loc_meas = [Q_loc_meas; 60];
    else
        Q_loc_meas = [Q_loc_meas; y];
    end
    vel = [vel; Q(2)];
    %iteratively plot what the ninja sees
    plot(0:dt:t, Q_loc, '-r.')
    plot(0:dt:t, Q_loc_meas, '-k.')
    plot(0:dt:t, Q_loc_no_noise, ':b')
    axis([0 duration -20 (max(max(Q_loc, Q_loc_meas))+10)])
    hold on
    pause(0.025)
end
pause
%plot theoretical path of ninja that doesn't use kalman
plot(0:dt:t, smooth(Q_loc_meas), '-g.')
pause
%plot velocity, just to show that it's constantly increasing, due to
%constant acceleration
%figure(2);
%plot(0:dt:t, vel, '-b.')

figure(2);
tt = 0 : dt : duration;
plot(tt,Q_loc,'-r.',tt,Q_loc_meas,'-k.');
hold on
axis([0 duration -20 (max(max(Q_loc, Q_loc_meas))+10)])


%% Do kalman filtering
%initize estimation variables
Q_loc_estimate = []; %  Quail position estimate
vel_estimate = []; % Quail velocity estimate
Q= [0; 0]; % re-initized state
P_estimate = P;
P_mag_estimate = [];
predic_state = [];
predic_var = [];
for t = 1:length(Q_loc)
    % Predict next state of the quail with the last state and predicted motion.
    Q_estimate = A * Q_estimate + B * u;
    predic_state = [predic_state; Q_estimate(1)] ;
    %predict next covariance
    P = A * P * A' + Ex;
    predic_var = [predic_var; P] ;
    % predicted Ninja measurement covariance
    % Kalman Gain
    K = P*C'*inv(C*P*C'+Ez);
    % Update the state estimate.
    if( mod(t, 20) == 0)
        %Q_estimate(1) = Q_loc_meas(t);
        Q_estimate = Q_estimate + K * (Q_loc_meas(t) - C * Q_estimate);
    else
        Q_estimate = Q_estimate + K * (Q_loc_meas(t) - C * Q_estimate);
    end
    if (randi(20) == 1 )
        bit_flip_pos = int32(bitshift(1, randi(12)))
        Q_estimate
        t/10
        v_or_p = 1
        Q_simulate_flip = int32(Q_estimate(v_or_p)*128)
        Q_simulate_flip = bitxor(Q_simulate_flip, bit_flip_pos)
        Q_simulate_flip = Q_simulate_flip/128.00
        Q_estimate(v_or_p) = Q_simulate_flip
        plot((t-1)/10, Q_simulate_flip, 'o', 'MarkerSize', 10)
        pause
    end
    % update covariance estimation.
    P =  (eye(2)-K*C)*P;


    %Store for plotting
    Q_loc_estimate = [Q_loc_estimate; Q_estimate(1)];
    vel_estimate = [vel_estimate; Q_estimate(2)];
    P_mag_estimate = [P_mag_estimate; P(1)];

    plot(0:dt:(t-1)/10, Q_loc_estimate, '-g')
    pause(0.025)
end

% Plot the results
% figure(2);
% tt = 0 : dt : duration;
% plot(tt,Q_loc,'-r.',tt,Q_loc_meas,'-k.', tt,Q_loc_estimate,'-g.');
% axis([0 duration -20 150])


%plot the evolution of the distributions
% figure(3);clf
% for T = 1:length(Q_loc_estimate)
% clf
%     x = Q_loc_estimate(T)-5:.01:Q_loc_estimate(T)+5; % range on x axis
%      
%     %predicted next position of the quail    
%     hold on
%     mu = predic_state(T); % mean
%     sigma = predic_var(T); % standard deviation
%     y = normpdf(x,mu,sigma); % pdf
%     y = y/(max(y));
%     hl = line(x,y,'Color','m'); % or use hold on and normal plot
%       
%     %data measured by the ninja
%     mu = Q_loc_meas(T); % mean
%     sigma = NinjaVision_noise_mag; % standard deviation
%     y = normpdf(x,mu,sigma); % pdf
%     y = y/(max(y));
%     hl = line(x,y,'Color','k'); % or use hold on and normal plot
%    
%     %combined position estimate
%     mu = Q_loc_estimate(T); % mean
%     sigma = P_mag_estimate(T); % standard deviation
%     y = normpdf(x,mu,sigma); % pdf
%     y = y/(max(y));
%     hl = line(x,y, 'Color','g'); % or use hold on and normal plot
%     axis([Q_loc_estimate(T)-5 Q_loc_estimate(T)+5 0 1]);    
% 
%    
%     %actual position of the quail
%     plot(Q_loc(T));
%     ylim=get(gca,'ylim');
%     line([Q_loc(T);Q_loc(T)],ylim.','linewidth',2,'color','b');
%     legend('state predicted','measurement','state estimate','actual Quail position')
%     pause
%end