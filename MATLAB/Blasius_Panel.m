% This file adds in viscous effects by modelling a boundary layer
% across the surfaces of the airfoil using the Blasius approximations.
% 
% Currently, only laminar, attached flow is modelled.

% X:            X coordinates of the geometry nodes
% Y:            Y coordinates of the geometry nodes
% Vinf:         Fluid Freestream velocity
% alpha:        Angle of attack of geometry to freestream
% mu:           Fluid Dynamic viscosity
% rho:          Fluid Density
% it:           Max number of iterations to complete
% b_plotFoil:   Show airfoil and boundary layers plot
% b_plotConv:   Show boundary layer convergence plot
function [Cp,Cf,strengths,X_mid,Y_mid,norms,delta,delta_star] = Blasius_Panel(X,Y,Vinf,alpha,mu,rho,it,b_plotFoil,b_plotConv)

% Set the geometry coordinates
X_base = X;
Y_base = Y;

% Perform an inviscid analysis to get the stagnation point
[Cp,strengths,~,~,~] = inviscidPanel(X,Y,Vinf,alpha);
[~,I] = max(Cp);

% Get the node normals and split them into top and bottom surfaces
norms_base = getNorms(X_base,Y_base);
norms_top_base = norms_base(I:end,:);
norms_bot_base = flip(norms_base(1:I,:));

% Distance from the nodes to sample the velocity
choice_dist = 1e-10;

% Local skin friction coefficient
Cf = zeros(length(X)-1,2);

% Convergence variable for determining if the solution converged (and
% plotting convergence of the boundary layer)
conv_var = zeros(it,1);

fprintf("Starting...\n")
% 1. Panel method gives u at body
% 2. Solve for delta_star at those u
% 3. Get new body with those delta_star
% 4. Repeat 1-3
for k = 1:it
    if k ~= 1
        %% Inviscid Panel Method
        [~,strengths,~,~,~] = inviscidPanel(X,Y,Vinf,alpha);
    end
    
    % Get the node normals for the current effective geometry
    norms_n = getNorms(X,Y);
    % Split into top and bottom sections
    norms_top = norms_n(I:end,:);
    norms_bot = flip(norms_n(1:I,:));
    
    %% Create and analyze boundary layer on top & bottom surfaces
    clear x v
    Re_x = @(x,v) abs((rho*v/mu) * x);          % Reynolds Number from chord location
    
    % Split nodes into top and bottom sections
    X_top = X(I:end)';
    Y_top = Y(I:end)';
    X_bot = flip(X(1:I)');
    Y_bot = flip(Y(1:I)');
    S_top = 0;                                  % Total panel length so far (top surface)
    S_bot = 0;                                  % Total panel length so far (bottom surface)
    deltas_top = zeros(length(X_top)-1,2);      % Boundary layer height (top)
    delta_d_top = deltas_top;                   % Displacement boundary layer height (top)
    deltas_bot = zeros(length(X_bot)-1,2);      % Boundary layer height (bottom)
    delta_d_bot = deltas_bot;                   % Displacement boundary layer height (bottom)
    
    %% Top surface
    for i = 1:length(X_top)-1
        % Make variables for panel nodes for easier access
        xii = X_top(i + 1);
        xi = X_top(i);
        x0 = 0.001;
        yii = Y_top(i + 1);
        yi = Y_top(i);
        dx = xii - xi;
        dy = yii - yi;
        len = sqrt(dx^2 + dy^2);    % Panel length
        
        % BL thickness in local panel coordinates
        delta_star = 5.*(x0+S_top)./sqrt(Re_x(x0+S_top,getVel(xi+norms_top(i,1)*choice_dist,xi + norms_top(i,2)*choice_dist,X,Y,strengths,Vinf,alpha)));
        % Transform thickness to global coordinates then
        %   calculate boundary layer location by adding above to geometry
        deltas_top(i,:) = delta_star*norms_top_base(i,:) + [X_base(I + i - 1) Y_base(I + i - 1)];
        
        % BL displacement thickness in local panel coordinates
        delta_d_star = 1.72.*(x0+S_top)./sqrt(Re_x(x0+S_top,getVel(xi+norms_top(i,1)*choice_dist,xi + norms_top(i,2)*choice_dist,X,Y,strengths,Vinf,alpha)));
        % Transform displacement thickness to global coordinates then
        % 	calculate boundary layer location by adding above to geometry
        delta_d_top(i,:) = delta_d_star*norms_top_base(i,:) + [X_base(I + i - 1) Y_base(I + i - 1)];
        
        % Calculate the skin friction coefficient contribution of this
        % panel
        xx = linspace(x0+S_top, x0+S_top+len, 20)';
        v_ave = (getVel(xi+norms_top(i,1)*choice_dist,xi + norms_top(i,2)*choice_dist,X,Y,strengths,Vinf,alpha) + ...
            getVel(xii+norms_top(i,1)*choice_dist,xii + norms_top(i,2)*choice_dist,X,Y,strengths,Vinf,alpha))/2;
        ti = atan2(yii-yi,xii-xi);          % Angle between x-axis and panel
        
        cf = .664/len * trapz(xx,Re_x(xx,v_ave).^-0.5);
        Cf(i+I-1,:) = [cf*cos(ti) cf*sin(ti)];
        
        % Add to total length travelled
        S_top = S_top + len;
    end
    
    %% Bottom surface
    for i = 1:length(X_bot)-1
        % Make variables for panel nodes for easier access
        xii = X_bot(i + 1);
        xi = X_bot(i);
        yii = Y_bot(i + 1);
        yi = Y_bot(i);
        dx = xii - xi;
        dy = yii - yi;
        len = sqrt(dx^2 + dy^2);        % Panel length
        
        % BL thickness in local panel coordinates
        delta_star = 5.*(x0+S_bot)./sqrt(Re_x(x0+S_bot,getVel(xi+norms_bot(i,1)*choice_dist,xi + norms_bot(i,2)*choice_dist,X,Y,strengths,Vinf,alpha)));
        % Transform thickness to global coordinates then
        %   calculate boundary layer location by adding above to geometry
        deltas_bot(i,:) = delta_star*norms_bot_base(i,:) + [X_base(I - i + 1) Y_base(I - i + 1)];
        
        % BL displacement thickness in local panel coordinates
        delta_d_star = 1.72.*(x0+S_bot)./sqrt(Re_x(x0+S_bot,getVel(xi+norms_bot(i,1)*choice_dist,xi + norms_bot(i,2)*choice_dist,X,Y,strengths,Vinf,alpha)));
        % Transform displacement thickness to global coordinates then
        %   calculate boundary layer location by adding above to geometry
        delta_d_bot(i,:) = delta_d_star*norms_bot_base(i,:) + [X_base(I - i + 1) Y_base(I - i + 1)];
        
        % Calculate the skin friction coefficient contribution of this
        % panel
        xx = linspace(x0+S_bot, x0+S_bot+len, 20)';
        v_ave = (getVel(xi+norms_bot(i,1)*choice_dist,xi + norms_bot(i,2)*choice_dist,X,Y,strengths,Vinf,alpha) + ...
            getVel(xii+norms_bot(i,1)*choice_dist,xii + norms_bot(i,2)*choice_dist,X,Y,strengths,Vinf,alpha))/2;
        ti = atan2(yii-yi,xii-xi);          % Angle between x-axis and panel
        
        cf = .664/len * trapz(xx,Re_x(xx,v_ave).^-0.5);
        Cf(I-i,:) = [cf*cos(ti) cf*sin(ti)];
        
        % Add to total length travelled
        S_bot = S_bot + len;
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
    
    % Setup variables for convergence plot
    delta_star = [flip(delta_d_bot); delta_d_top(2:end,:)];
    for j = 1:length(delta_star)
        delta_star(j,1) = norm(delta_star(j,:));
    end
    delta_star(:,2) = [];
    conv_var(k) = 1;
    if k > 1
        conv_var(k) = (norm(delta_star)-conv_var_old)/conv_var_old;
    end
    conv_var_old = norm(delta_star);
    % If convergence is within 1e-10, stop
    if abs(conv_var(k)) <= 1e-10
        break
    end
    
    % Set new effective body to the displacement thickness
    X = [flip(delta_d_bot(:,1)); delta_d_top(2:end,1)];
    Y = [flip(delta_d_bot(:,2)); delta_d_top(2:end,2)];
    
    fprintf("%d\n",k)
    
end
conv_var = conv_var(1:k);
fprintf("Done!\n")
clc

% Store the boundary and displacement height coordinates
delta = [flip(deltas_bot); deltas_top(2:end,:)];
delta_star = [flip(delta_d_bot); delta_d_top(2:end,:)];

% Get the final Cp and panel strengths from an inviscid method
[Cp,strengths,X_mid,Y_mid,norms] = inviscidPanel(X,Y,Vinf,alpha);

% Plot final boundary layer thicknesses and the stagnation point
if b_plotFoil
    figure()
    p(1) = plot(X_base,Y_base,'k.-');   % Geomtry
    hold on
    p(2) = plot(deltas_top(:,1),deltas_top(:,2),'r');   % BL Height Top
    p(3) = plot(delta_d_top(:,1),delta_d_top(:,2),'b'); % Displacement Height Top
    plot(deltas_bot(:,1),deltas_bot(:,2),'r')           % BL Height Bottom
    plot(delta_d_bot(:,1),delta_d_bot(:,2),'b')         % Displacement Height Bottom
    p(4) = plot(X_base(I),Y_base(I),'m*');  % Stagnation Point
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
    plot(conv_var,'.-','linewidth',2,'markersize',16)
    xlabel("Iteration \#",'interpreter','latex')
    title("$\delta^*$ Convergence",'interpreter','latex')
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
