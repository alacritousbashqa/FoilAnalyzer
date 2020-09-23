%% Panel method code based on theory presented in Virginia Tech pdf (http://www.dept.aoe.vt.edu/~mason/Mason_f/CAtxtChap4.pdf)
% Uses source panels of constant and different strength for each panel, and
% vortex panels of constant and equal strength for each panel
%
% Thus, we need to solve for N source strengths and 1 vortex strength using N+1 equations.

% X:            X coordinates of the geometry nodes
% Y:            Y coordinates of the geometry nodes
% Vinf:         Fluid Freestream velocity
% alpha:        Angle of attack of geometry to freestream
function [Cp,strengths,X_mid,Y_mid,norms,tans] = inviscidPanel(X,Y,Vinf,alpha)

M = length(X);          % Number of points
N = M-1;                % Number of panels

X_mid = zeros(N,1);     % Holds all control point X locations
Y_mid = zeros(N,1);     % Holds all control point Y locations
norms = zeros(N,2);     % Holds all panel normals
tans = zeros(N,2);      % Holds all panel tangents
thetas = zeros(N,1);    % Holds all panel orientations with respect to the x-axis

%% Calculate influence coefficients

A = zeros(N+1,N+1);     % Influence coefficient matrix
c = A;
b = zeros(N+1,1);       % Freestream contribution

for i = 1:N
    % Coordinates of ith panel end and control points
    % Control Points: (x,y)
    % First End Point: (xi,yi)
    % Second End Point: (xii,yii)
    xi = X(i);
    xii = X(i+1);
    x = (xi+xii)/2;
    X_mid(i) = x;
    yi = Y(i);
    yii = Y(i+1);
    y = (yi+yii)/2;
    Y_mid(i) = y;
    
    ti = atan2(yii-yi,xii-xi);          % Angle between x-axis and panel
    thetas(i) = ti;
    norms(i,:) = [-sin(ti) cos(ti)];    % ith Panel normal
    tans(i,:) = [cos(ti) sin(ti)];      % ith Panel tangent
    
    % Tangential flow condition (Source contribution)
    for j = 1:N
        % Coordinates of jth panel end points
        % First End Point: (xj,yj)
        % Second End Point: (xjj,yjj)
        xj = X(j);
        xjj = X(j+1);
        yj = Y(j);
        yjj = Y(j+1);
        
        tj = atan2(yjj-yj,xjj-xj);                      % Angle between x-axis and panel
        
        % IN PANEL FRAME
        r0 = sqrt((y-yj)^2+(x-xj)^2);                   % Distance from (xj,yj) to control point
        r1 = sqrt((y-yjj)^2+(x-xjj)^2);                 % Distance from (xjj,yjj) to control point
        
        % IN PANEL FRAME
        beta = atan2((y-yjj)*(x-xj)-(x-xjj)*(y-yj)...
            ,(x-xjj)*(x-xj)+(y-yjj)*(y-yj));            % Angle between distance vectors defined by distances r0 and r1
        flog = log(r1/r0);                              % Natural log of the ratio of above distances r1/r0
        
        % Influence of source panel on itself
        if i==j
            beta = pi;
            flog = 0;
        end
        
        delta_t = ti-tj;                                % Difference between ith and jth panel orientations
        
        % Calculate influence coefficient and transform back to fixed frame
        A(i,j) = (0.5/pi) * (sin(delta_t)*flog + cos(delta_t)*beta);
        
        % Add to vortex calculation sum and transform back to fixed frame
        c(i,j) = (0.5/pi) * (flog*cos(delta_t) - beta*sin(delta_t));
        A(i,N+1) = A(i,N+1) + c(i,j);
        
        if i==1 || i==N
            % If we're on the trailing edge, add to Kutta condition
            A(N+1,j) = A(N+1,j) - c(i,j);
            A(N+1,N+1) = A(N+1,N+1) + A(i,j);
        end
        
    end
    
    % Freestream flow contribution
    b(i) = Vinf*sin(ti-alpha);
end

% Freestream flow contribution
b(N+1) = -Vinf*cos(thetas(1)-alpha)-Vinf*cos(thetas(end)-alpha);

%% Solve linear system for strengths
strengths = A\b;
q = strengths(1:end-1);
gamma = strengths(end);

%% Get Cp and tangential velocities

Cp = zeros(N,1);
vtan = zeros(N,1);
for i=1:N
    vtan(i) = cos(thetas(i)-alpha)*Vinf;
    for j=1:N
        vtan(i) = vtan(i) - c(i,j)*q(j) + gamma*A(i,j);
    end
    Cp(i) = 1-(vtan(i)/Vinf)^2;
end