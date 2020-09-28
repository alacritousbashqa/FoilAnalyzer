% This file adds in viscous effects by modelling a boundary layer
% across the surfaces of the airfoil based on Thwaites method.

% X:            X coordinates of the geometry nodes
% Y:            Y coordinates of the geometry nodes
% Vinf:         Fluid Freestream velocity
% alpha:        Angle of attack of geometry to freestream
% mu:           Fluid Dynamic viscosity
% rho:          Fluid Density
% it:           Number of iterations to complete
% b_plotFoil:   Show airfoil and boundary layers plot
% b_plotConv:   Show boundary layer convergence plot
function [A,Cp,Cf,strengths,X_mid,Y_mid,norms] = Thwaites_Panel(X,Y,Vinf,alpha,mu,rho,it,b_plotFoil,b_plotConv)

nu = mu/rho;    % Kinematic Viscosity [m^2/s]
% Get the table of parameters as presented by Thwaites
[numbers] = xlsread("Thwaites Table.xlsx");
m_table = numbers(1:end,1);
l_table = numbers(1:end,2);
H_table = numbers(1:end,3);
L_table = numbers(1:end,4);
% Get the table of parameters as presented by Polhausen
[numbers] = xlsread("Polhausen Table.xlsx");
lambda_table = numbers(1:end,1);
K_table = numbers(1:end,2);
H_P_table = numbers(1:end,4);


% Set the geometry coordinates
X_base = X;
Y_base = Y;

% Perform an inviscid analysis to get the stagnation point
[Cp,strengths,X_mid,Y_mid,norms] = inviscidPanel(X,Y,Vinf,alpha);
[~,I] = max(Cp.*(X_mid<0.5));

% Get the node normals and split them into top and bottom surfaces
norms_node = getNorms(X_base,Y_base);
norms_top_node = norms_node(I:end,:);
norms_bot_node = flip(norms_node(1:I,:));

BL_heights = zeros(it,1);

% Distance from the mids to sample the velocity
sample_dist = 1e-10;

Cf = zeros(length(X)-1,2);

% 1. Panel method gives u at body
% 2. Solve for delta_star at those u
% 3. Get new body with those delta_star
% 4. Repeat 1-3
for k = 1:it
    if k ~= 1
        %% Inviscid Panel Method
        [~,strengths,~,~,~] = inviscidPanel(X,Y,Vinf,alpha);
    end
    
    % Get the normals at the panel mids for the top and bottom surfaces
    norms_top = norms(I:end,:);
    norms_bot = norms(1:I,:);
    
    %% Create and analyze boundary layer on top & bottom surfaces
    
    % Split panel mids into top and bottom sections
    X_top = X_mid(I:end)';
    Y_top = Y_mid(I:end)';
    X_bot = flip(X_mid(1:I)');
    Y_bot = flip(Y_mid(1:I)');
    % Split nodes into top and bottom sections
    X_top_node = X(I:end)';
    Y_top_node = Y(I:end)';
    X_bot_node = flip(X(1:I)');
    Y_bot_node = flip(Y(1:I)');
    % Surface lengths
    S_top = 0;                                  % Total panel length so far (top surface)
    S_bot = 0;                                  % Total panel length so far (bottom surface)
    % Boundary layer heights
    deltas_top = zeros(length(X_top)-1,2);      % Boundary layer height (top)
    delta_d_top = deltas_top;                   % Displacement boundary layer height (top)
    delta_d_top_P = deltas_top;                 % "   "   "   "   "   "   "   "   "   "   " (Polhausen)
    deltas_bot = zeros(length(X_bot)-1,2);      % Boundary layer height (bottom)
    delta_d_bot = deltas_bot;                   % Displacement boundary layer height (bottom)
    delta_d_bot_P = deltas_bot;                 % "   "   "   "   "   "   "   "   "   "   " (Polhausen)
    
    % Vectors of velocity, its derivative, and the position along the
    % surface
    velocs = zeros(length(X_top),1);
    pos = velocs;
    Re_t = velocs;
    
    %% Top surface
    for i = 1:length(X_top)-1
        % Make variables for panel nodes for easier access
        xi = X_top(i);
        yi = Y_top(i);
        dx = X_top_node(i + 1) - X_top_node(i);
        dy = Y_top_node(i + 1) - Y_top_node(i);
        len = sqrt(dx^2 + dy^2);    % Panel length
        
        % Store the position (length along surface)
        pos(i) = S_top + len/2;
        
        vi = getVel(xi + norms_top(i,1)*sample_dist,yi + norms_top(i,2)*sample_dist,X,Y,strengths,Vinf,alpha);
        
        % Store the velocity
        velocs(i) = vi;
        
        % Add to total length travelled
        S_top = S_top + len;
    end
    
    % Store the values for the last point
    pos(end) = S_top;
    
    d_velocs = diff(velocs)./diff(pos);
    d_velocs = [d_velocs; d_velocs(end)];
    
    % Get the momentum thickness, theta, from the velocities
    theta = zeros(length(pos),1);
    thetaP = zeros(length(pos),1);
    AA = theta;
    for i = 1:length(velocs)
        if i == 1
            theta(i) = 0;
        else
            AA(i) = trapz(pos(1:i),velocs(1:i).^5);
            theta(i) = (0.441*nu*(velocs(i))^-6)*trapz(pos(1:i),velocs(1:i).^5);
            theta(i) = sqrt(theta(i));
            
            thetaP(i) = (0.47*nu*(velocs(i))^-6)*trapz(pos(1:i),velocs(1:i).^5);
            thetaP(i) = sqrt(thetaP(i));
        end
        % Momentum thickness Reynold Number
        Re_t(i) = velocs(i)*theta(i)/nu;
    end
    
    
    %========================================================================
    %       DEBUG PLOTS
    figure(6)
    plot(X_top,1-(velocs/Vinf).^2)
    xlabel("X")
    set(gca, 'YDir','reverse')
    
    figure(7)
    plot(X_top(10:end-10),d_velocs(10:end-10),'r')
    xlabel("X")
    title("dv/dx")
    
    figure(5)
    plot(X_top,velocs,'r')
    title("Velocities")
    %========================================================================
    
    
    
    
    % Surface length Reynold Number
    Re_x = velocs.*pos./nu;
    
    % Get the first derivative value and initial value for the momentum
    % thickness, theta0
    vd0 = d_velocs(1);
    vd1 = d_velocs(2);
    vdd0 = (vd1-vd0)/(pos(2)-pos(1));
    theta(1) = theta(2)^2 - (-.45*mu/rho/7*vdd0/vd0^2)*(pos(2)-pos(1));
    theta(1) = sqrt(abs(theta(1)));
    theta0 = theta(1);
    
    m = -(theta.^2).*d_velocs/nu;           % The parameter defined by Thwaites
    K = (theta.^2).*d_velocs/nu;
    H = zeros(length(m),1);                 % Shape factor
    H_P = H;
    L = H;                                  % L = 2[(H + 2)m + l]
    l = L;                                  % Relates to the skin friction
    for i = 1:length(m)
        % Get the L and H corresponding to each m based on the table
        % presented by Thwaites
        H(i) = interp1(m_table,H_table,m(i));
        L(i) = interp1(m_table,L_table,m(i));
        l(i) = interp1(m_table,l_table,m(i));
        
        H_P(i) = interp1(K_table,H_P_table,K(i));
    end
    
    % Debugging variable
    A = zeros(length(m),10);
    A(:,1) = velocs;
    A(:,2) = d_velocs;
    A(:,3) = theta.^2;
    A(:,4) = m;
    A(:,5) = H;
    A(:,6) = AA;
    A(:,7) = pos;
    A(:,8) = l;
    A(:,9) = K;
    A(1,10) = min(pos(Re_t > 1.174.*(1+22400./Re_x).*Re_x.^0.46));   % Transition Point
    
    % Get the momentum thickness global coordinates for the top surface
    theta_top = zeros(length(theta),2);
    for i = 1:length(delta_d_top)
        theta_top(i,:) = theta(i)*norms_top_node(i,:) + [X_top(i) Y_top(i)];
    end
    
    delta_d_star = H.*theta;            % Displacement thickness in the panel frames
    delta_d_star_P = H_P.*thetaP;
    % Convert displacement thickness from panel frames to fixed frame
    for i = 1:length(delta_d_top)
        delta_d_top(i,:) = delta_d_star(i)*norms_top_node(i,:) + [X_top(i) Y_top(i)];
        delta_d_top_P(i,:) = delta_d_star_P(i)*norms_top_node(i,:) + [X_top(i) Y_top(i)];
    end
    
    % Plot boundary layer thickness and shape factor
    figure(50)
    plot(pos,theta*sqrt(max(X)*Vinf/nu),'r');
    xlabel("x (surface length)")
    title("Boundary Layer Parameters Normalized with Re_c")
    ylim([0 5])
    xlim([0 1.1])
    hold on
    plot(pos,thetaP*sqrt(max(X)*Vinf/nu),'r--')
    plot(pos,0.665*sqrt(nu*pos./velocs)*sqrt(max(X)*Vinf/nu),'r-.')
    plot(pos,H,'m')
    plot(pos,H_P,'m--')
    plot(pos,ones(length(pos),1)*2.586,'m-.')
    plot(pos,delta_d_star*sqrt(max(X)*Vinf/nu),'b')
    plot(pos,delta_d_star_P*sqrt(max(X)*Vinf/nu),'b--')
    plot(pos,1.72*sqrt(nu*pos./velocs)*sqrt(max(X)*Vinf/nu),'b-.')
    hold off
    legend('$\theta \sqrt{Re}$','$\theta \sqrt{Re}$ (Polhausen)','$\theta \sqrt{Re}$ (Blasius)',...
        'H','H (Polhausen)','H (Blasius)',...
        '$\delta^\ast \sqrt{Re}$','$\delta^\ast \sqrt{Re}$ (Polhausen)','$\delta^\ast \sqrt{Re}$ (Blasius)',...
        'location','northeast','interpreter','latex')
    
    % Clear for use on the bottom surface
    velocs = zeros(length(X_bot),1);
    d_velocs = velocs;
    pos = velocs;
    
    %% Bottom surface
    for i = 1:length(X_bot)-1
        % Make variables for panel nodes for easier access
        xi = X_bot(i);
        yi = Y_bot(i);
        dx = X_bot_node(i + 1) - X_bot_node(i);
        dy = Y_bot_node(i + 1) - Y_bot_node(i);
        len = sqrt(dx^2 + dy^2);    % Panel length
        
        % Store the position (length along surface)
        pos(i) = S_bot + len/2;
        
        vi = getVel(xi + norms_bot(i,1)*sample_dist,yi + norms_bot(i,2)*sample_dist,X,Y,strengths,Vinf,alpha);
        
        % Store the velocity
        velocs(i) = vi;
        
        % Add to total length travelled
        S_bot = S_bot + len;
    end
    
    % Store the values for the last point
    pos(end) = S_bot;
    
    d_velocs = diff(velocs)./diff(pos);
    d_velocs = [d_velocs; d_velocs(end)];
    
    % Get the momentum thickness, theta, from the velocities
    theta = zeros(length(pos),1);
    thetaP = zeros(length(pos),1);
    AA = theta;
    for i = 1:length(velocs)
        if i == 1
            theta(i) = 0;
        else
            AA(i) = trapz(pos(1:i),velocs(1:i).^5);
            theta(i) = (0.441*nu*(velocs(i))^-6)*trapz(pos(1:i),velocs(1:i).^5);
            theta(i) = sqrt(theta(i));
            
            thetaP(i) = (0.47*nu*(velocs(i))^-6)*trapz(pos(1:i),velocs(1:i).^5);
            thetaP(i) = sqrt(thetaP(i));
        end
    end
    theta(1) = theta0;
    
    m = -(theta.^2).*d_velocs/nu;           % The parameter defined by Thwaites
    K = (theta.^2).*d_velocs/nu;
    H = zeros(length(m),1);                 % Shape factor
    H_P = H;
    L = H;                                  % L = 2[(H + 2)m + l]
    l = L;                                  % Relates to the skin friction
    for i = 1:length(m)
        % Get the L and H corresponding to each m based on the table
        % presented by Thwaites
        H(i) = interp1(m_table,H_table,m(i));
        L(i) = interp1(m_table,L_table,m(i));
        l(i) = interp1(m_table,l_table,m(i));
        
        H_P(i) = interp1(K_table,H_P_table,K(i));
    end
    
    % Get the momentum thickness global coordinates for the top surface
    theta_bot = zeros(length(theta),2);
    for i = 1:length(delta_d_bot)
        theta_bot(i,:) = theta(i)*norms_bot_node(i,:) + [X_bot(i) Y_bot(i)];
    end
    
    delta_d_star = H.*theta;            % Displacement thickness in the panel frames
    delta_d_star_P = H_P.*thetaP;
    % Convert displacement thickness from panel frames to fixed frame
    for i = 1:length(delta_d_bot)
        delta_d_bot(i,:) = delta_d_star(i)*norms_bot_node(i,:) + [X_bot(i) Y_bot(i)];
        delta_d_bot_P(i,:) = delta_d_star_P(i)*norms_bot_node(i,:) + [X_bot(i) Y_bot(i)];
    end
    
    % Current method for trailing edge, meant to keep the analyzed mesh
    %   closed
    slope = (delta_d_top(end,2)-delta_d_top(end-1,2))/(delta_d_top(end,1)-delta_d_top(end-1,1));
    if slope==0
        delta_d_top = [delta_d_top; 1.2 0];
        delta_d_bot = [delta_d_bot; 1.2 0];
    else
        dx_int = -delta_d_top(end,2)/(1.2*slope);
        delta_d_top = [delta_d_top; delta_d_top(end,1)+dx_int 0];
        delta_d_bot = [delta_d_bot; delta_d_top(end-1,1) delta_d_bot(end,2)+.002];
    end
    delta_d_bot(end,:) = delta_d_top(end,:);
    
    
    
    figure(6)
    hold on
    plot(X_bot,1-(velocs/Vinf).^2)
    set(gca, 'YDir','reverse')
    title("Cp")
    hold off
    
    
    
    % Plot the displacement thickness, geometry, and stagnation point
    figure(2)
    plot(X_base,Y_base,'k.-');
    hold on
    plot(delta_d_top(:,1),delta_d_top(:,2),'b');
    plot(delta_d_top_P(:,1),delta_d_top_P(:,2),'r')
    plot(X_base(I),Y_base(I),'m*');
    pbaspect([1.4 1.0 1])
    xlim([-.2 1.2])
    ylim([-.5 .5])
    xlabel("x/c")
    ylabel("y/c")
    title("Airfoil Boundary Layers")
    plot(0,-.4,'ro')
    plot([0 0.1*cos(alpha)], [-.4 0.1*sin(alpha)-.4],'r')
    hold off
    
    
    % Set new effective body to the displacement thickness
    X = [flip(delta_d_bot(:,1)); delta_d_top(2:end,1)];
    Y = [flip(delta_d_bot(:,2)); delta_d_top(2:end,2)];
    
end

% Get the final Cp and panel strengths from an inviscid method
[Cp,strengths,X_mid,Y_mid,norms] = inviscidPanel(X,Y,Vinf,alpha);

% Plot final boundary layer thicknesses and the stagnation point
if b_plotFoil
    figure(2)
    p(1) = plot(X_base,Y_base,'k.-');
    hold on
    p(2) = plot(deltas_top(:,1),deltas_top(:,2),'r');
    p(3) = plot(delta_d_top(:,1),delta_d_top(:,2),'b');
    plot(deltas_bot(:,1),deltas_bot(:,2),'r')
    plot(delta_d_bot(:,1),delta_d_bot(:,2),'b')
    p(4) = plot(X_base(I),Y_base(I),'m*');
    pbaspect([1.4 1.0 1])
    xlim([-.2 1.2])
    ylim([-.5 .5])
    xlabel("x/c")
    ylabel("y/c")
    title("Airfoil Boundary Layers")
    hold off
    legend([p(1) p(2) p(3) p(4)],'Geometry','\delta','\delta^*','Stagnation Point')
end

% Plot of displacement BL height at a station vs iteration number
if b_plotConv
    figure()
    plot(BL_heights)
    ylim([0 max(BL_heights)])
    xlabel("Iteration #")
    ylabel("\delta^*")
    title("\delta^* Convergence")
    grid on
end

end



% ==========================================================================
%               AUXILIARY FUNCTIONS
% ==========================================================================

%% Gets the node normals across the entire airfoil
function [norms_n] = getNorms(X,Y)

norms_n = zeros(length(X),2);

for i = 1:length(X)
    % Set variables for the points before, at, and after the index
    %   Also adjust for nodes at the beginning and end
    if i == 1               % If the first node
        x = X(end-1);
        y = Y(end-1);
        xi = X(i);
        yi = Y(i);
        xii = X(i+1);
        yii = Y(i+1);
    elseif i == length(X)   % If the last node
        x = X(i-1);
        y = Y(i-1);
        xi = X(i);
        yi = Y(i);
        xii = X(2);
        yii = Y(2);
    else                    % If not first/last node
        x = X(i-1);
        y = Y(i-1);
        xi = X(i);
        yi = Y(i);
        xii = X(i+1);
        yii = Y(i+1);
    end
    
    % Get the normals of the panels before and after the node
    t1 = atan2(yi-y,xi-x);
    t2 = atan2(yii-yi,xii-xi);
    norm1 = [-sin(t1) cos(t1)];
    norm2 = [-sin(t2) cos(t2)];
    
    % Average the panel normals to get the node normal
    norms_n(i,:) = (norm1 + norm2)./2;
    norms_n(i,:) = norms_n(i,:)/norm(norms_n(i,:));
end

end

%% Gets velocity at a point induced by the freestream and the panels
function [V,u,v] = getVel(x,y,X,Y,strengths,Vinf,alpha)

u = 0;
v = 0;
N = length(X)-1;

for j = 1:N
    % Coordinates of jth panel end points
    % First End Point: (xj,yj)
    % Second End Point: (xjj,yjj)
    xj = X(j);
    xjj = X(j+1);
    yj = Y(j);
    yjj = Y(j+1);
    
    tj = atan2(yjj-yj,xjj-xj);          % Angle between x-axis and panel
    
    % IN PANEL FRAME
    r0 = sqrt((y-yj)^2+(x-xj)^2);                   % Distance from (xj,yj) to control point
    r1 = sqrt((y-yjj)^2+(x-xjj)^2);                 % Distance from (xjj,yjj) to control point
    
    
    % IN PANEL FRAME
    beta = atan2((y-yjj)*(x-xj)-(x-xjj)*(y-yj)...
        ,(x-xjj)*(x-xj)+(y-yjj)*(y-yj));            % Angle between distance vectors defined by distances r0 and r1
    flog = log(r1/r0);                              % Natural log of the ratio of above distances r1/r0
    
    us_star = -(0.5/pi)*flog;
    vs_star = beta/2/pi;
    vv_star = (0.5/pi)*flog;
    uv_star = vs_star;
    
    if (0.5*(xj+xjj) == x) && (0.5*(yj+yjj) == y)
        uv_star = 0.5;
        vv_star = 0;
        us_star = 0;
        vs_star = 0.5;
    end
    
    u = u + strengths(j)*(cos(tj)*us_star - sin(tj)*vs_star) + strengths(end)*(uv_star*cos(tj) - vv_star*sin(tj));
    v = v + strengths(j)*(sin(tj)*us_star + cos(tj)*vs_star) + strengths(end)*(uv_star*sin(tj) + vv_star*cos(tj));
end

u = u + Vinf*cos(alpha);
v = v + Vinf*sin(alpha);

V = sqrt(u^2 + v^2);
end
