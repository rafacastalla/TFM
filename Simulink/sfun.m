function [sys,x0,str,ts,simStateCompliance] = sfun(t,x,u,flag,s_baudrate,s_type,s_length)

% https://es.mathworks.com/matlabcentral/answers/35270-s-function-for-reading-com-port

persistent NMEA DD;
if isempty(NMEA) %true only the first time and config serial Port
    NMEA = serial('/dev/ttyACM0', 'BaudRate', s_baudrate);
    NMEA.InputBufferSize=2048;
    NMEA.OutputBufferSize=2048;
end

switch flag,
    
    %%%%%%%%%%%%%%%%%%
    % Initialization %
    %%%%%%%%%%%%%%%%%%
    case 0,
        [sys,x0,str,ts,simStateCompliance]=mdlInitializeSizes(s_length);
        DD=zeros(4,1);
        fopen(NMEA);
        
        %%%%%%%%%%%
        % Outputs %
        %%%%%%%%%%%
    case 3,
        % Read serial
        syncend = 0;
        while(syncend==0)
            if(NMEA.BytesAvailable>0)
                sync=fread(NMEA,1);
                if(sync==126) 
                    if(NMEA.BytesAvailable>=9)
                        buffer=fread(NMEA,9);
                        if(buffer(9,1)==125)
                            % Paquete syncronizado
                            % Comprobar si hay mas paquetes
                            x = buffer(1:2);
                            y = typecast(uint8(x), 'int16');
                            DD(1,1) = y;

                            x = buffer(3:4);
                            y = typecast(uint8(x), 'int16');
                            DD(2,1) = y;

                            x = buffer(5:6);
                            y = typecast(uint8(x), 'int16');
                            DD(3,1) = y;

                            x = buffer(7:8);
                            y = typecast(uint8(x), 'int16');
                            DD(4,1) = y;
                            if(NMEA.BytesAvailable<10)
                               syncend = 1; 
                            end
                        else
                            % No esta sincronizado el paquete
                            syncend = 1;
                        end
                    end
                end
            else
                syncend = 1;
            end
        end
        sys=[DD];
        
        
        %%%%%%%%%%%%%
        % Terminate %
        %%%%%%%%%%%%%
    case 9,
        sys=mdlTerminate(t,x,u);
        fclose(NMEA);
        delete(NMEA);
        
end

% end sfuntmpl

%
%=============================================================================
% mdlInitializeSizes
% Return the sizes, initial conditions, and sample times for the S-function.
%=============================================================================
%
function [sys,x0,str,ts,simStateCompliance]=mdlInitializeSizes(s_length)

%
% call simsizes for a sizes structure, fill it in and convert it to a
% sizes array.
%
% Note that in this example, the values are hard coded.  This is not a
% recommended practice as the characteristics of the block are typically
% defined by the S-function parameters.
%
sizes = simsizes;

sizes.NumContStates  = 0;
sizes.NumDiscStates  = 0;
sizes.NumOutputs     = s_length;
sizes.NumInputs      = s_length;
sizes.DirFeedthrough = 1;
sizes.NumSampleTimes = 1;   % at least one sample time is needed

sys = simsizes(sizes);

%
% initialize the initial conditions
%
x0  = [];

%
% str is always an empty matrix
%
str = [];

%
% initialize the array of sample times
%
ts  = [0 0];

% Specify the block simStateCompliance. The allowed values are:
%    'UnknownSimState', < The default setting; warn and assume DefaultSimState
%    'DefaultSimState', < Same sim state as a built-in block
%    'HasNoSimState',   < No sim state
%    'DisallowSimState' < Error out when saving or restoring the model sim state
simStateCompliance = 'UnknownSimState';

% end mdlInitializeSizes

%
%=============================================================================
% mdlDerivatives
% Return the derivatives for the continuous states.
%=============================================================================
%
function sys=mdlDerivatives(t,x,u)

sys = [];

% end mdlDerivatives

%
%=============================================================================
% mdlUpdate
% Handle discrete state updates, sample time hits, and major time step
% requirements.
%=============================================================================
%
function sys=mdlUpdate(t,x,u)

sys = [];

% end mdlUpdate

%
%=============================================================================
% mdlOutputs
% Return the block outputs.
%=============================================================================
%
function sys=mdlOutputs(t,x,u)

sys = [];

% end mdlOutputs

%
%=============================================================================
% mdlGetTimeOfNextVarHit
% Return the time of the next hit for this block.  Note that the result is
% absolute time.  Note that this function is only used when you specify a
% variable discrete-time sample time [-2 0] in the sample time array in
% mdlInitializeSizes.
%=============================================================================
%
function sys=mdlGetTimeOfNextVarHit(t,x,u)

sampleTime = 1;    %  Example, set the next hit to be one second later.
sys = t + sampleTime;

% end mdlGetTimeOfNextVarHit

%
%=============================================================================
% mdlTerminate
% Perform any end of simulation tasks.
%=============================================================================
%
function sys=mdlTerminate(t,x,u)

sys = [];

% end mdlTerminate
