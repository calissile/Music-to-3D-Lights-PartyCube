function varargout = Control(varargin)
    % CONTROL MATLAB code for Control.fig
    %      CONTROL, by itself, creates a new CONTROL or raises the existing
    %      singleton*.
    %
    %      H = CONTROL returns the handle to a new CONTROL or the handle to
    %      the existing singleton*.
    %
    %      CONTROL('CALLBACK',hObject,eventData,handles,...) calls the local
    %      function named CALLBACK in CONTROL.M with the given input arguments.
    %
    %      CONTROL('Property','Value',...) creates a new CONTROL or raises the
    %      existing singleton*.  Starting from the left, property value pairs are
    %      applied to the GUI before Control_OpeningFcn gets called.  An
    %      unrecognized property name or invalid value makes property application
    %      stop.  All inputs are passed to Control_OpeningFcn via varargin.
    %
    %      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
    %      instance to run (singleton)".
    %
    % See also: GUIDE, GUIDATA, GUIHANDLES

    % Edit the above text to modify the response to help Control

    % Last Modified by GUIDE v2.5 23-Nov-2013 15:58:14

    % Begin initialization code - DO NOT EDIT
    gui_Singleton = 1;
    gui_State = struct('gui_Name',       mfilename, ...
                       'gui_Singleton',  gui_Singleton, ...
                       'gui_OpeningFcn', @Control_OpeningFcn, ...
                       'gui_OutputFcn',  @Control_OutputFcn, ...
                       'gui_LayoutFcn',  [] , ...
                       'gui_Callback',   []);
    if nargin && ischar(varargin{1})
        gui_State.gui_Callback = str2func(varargin{1});
    end

    if nargout
        [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
    else
        gui_mainfcn(gui_State, varargin{:});
    end
    % End initialization code - DO NOT EDIT
end

    % --- Executes just before Control is made visible.
    function Control_OpeningFcn(hObject, eventdata, handles, varargin)
    % This function has no output args, see OutputFcn.
    % hObject    handle to figure
    % eventdata  reserved - to be defined in a future version of MATLAB
    % handles    structure with handles and user data (see GUIDATA)
    % varargin   command line arguments to Control (see VARARGIN)

    % Choose default command line output for Control
    handles.output = hObject;

    % Update handles structure
    guidata(hObject, handles);

    % UIWAIT makes Control wait for user response (see UIRESUME)
    % uiwait(handles.figure1);
    end

    % --- Outputs from this function are returned to the command line.
    function varargout = Control_OutputFcn(hObject, eventdata, handles) 
    % varargout  cell array for returning output args (see VARARGOUT);
    % hObject    handle to figure
    % eventdata  reserved - to be defined in a future version of MATLAB
    % handles    structure with handles and user data (see GUIDATA)

    % Get default command line output from handles structure
    varargout{1} = handles.output;
    end

    % --- Executes on button press in pushbutton1.
    function pushbutton1_Callback(hObject, eventdata, handles)
    % hObject    handle to pushbutton1 (see GCBO)
    status = system('cd C:\music-to-3d-master\HypnoDemo');
    status = system('HypnoDemo.exe COM5 1');
    pushbuttone1_Callback = status;
    % eventdata  reserved - to be defined in a future version of MATLAB
    % handles    structure with handles and user data (see GUIDATA)
    end

    % --- Executes on button press in pushbutton2.
    function pushbutton2_Callback(hObject, eventdata, handles)
    % hObject    handle to pushbutton2 (see GCBO)
    status = system('cd C:\music-to-3d-master\HypnoDemo');
    status = system('HypnoDemo.exe COM5 2');
    pushbuttone1_Callback = status;

    % eventdata  reserved - to be defined in a future version of MATLAB
    % handles    structure with handles and user data (see GUIDATA)
    end


    % --- Executes on button press in pushbutton3.
    function pushbutton3_Callback(hObject, eventdata, handles)
    % hObject    handle to pushbutton3 (see GCBO)
    status = system('cd C:\music-to-3d-master\HypnoDemo')
    status = system('HypnoDemo.exe COM5 3');
    pushbuttone1_Callback = status;

    % eventdata  reserved - to be defined in a future version of MATLAB
    % handles    structure with handles and user data (see GUIDATA)
    end


% --- Executes on button press in pushbutton5.
    function pushbutton5_Callback(hObject, eventdata, handles)
    % hObject    handle to pushbutton5 (see GCBO)
    % eventdata  reserved - to be defined in a future version of MATLAB
    % handles    structure with handles and user data (see GUIDATA)
        [FileName,PathName] = uigetfile('*.wav');
        filename = [PathName, FileName];
        filenametowrite3 = 'songname.txt';
        fileID3 = fopen(filenametowrite3,'w');    
        fprintf(fileID3, '%s', filename);
        fclose(fileID3);
        bps = fun1(filename);
    end
