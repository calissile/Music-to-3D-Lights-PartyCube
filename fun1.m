function [ output ] = fun1( filename )
%TEST1 Summary of this function goes here
%   Detailed explanation goes here

    [oriwave, samprate] = audioread(filename); %read in data
        if (size(oriwave,2)>1)    
            wavetopass(:,1) = oriwave(:,1);
        else
            wavetopass(:,1) = oriwave;
        end
        
    for j = 0:floor(length(wavetopass)/samprate)-1
        fullwaverect(:,1) = abs(wavetopass(1+j*samprate:samprate+j*samprate,1)); %full wave rectification
        
        %low pass filtering
        lowpassed(length(fullwaverect),1)=0;
        for i = 2: length(fullwaverect)
            lowpassed(i,1) = 0.5*fullwaverect(i)-0.5*fullwaverect(i-1);
        end

        %downsampling
        downsampled(length(lowpassed)/10,1)=0;
        for i = 1:length(lowpassed)/10
            downsampled(i,1) = lowpassed(10*i);
        end


        %count peaks
        count = 0;
        ex2 = mean(downsampled);
        for i = 4:length(downsampled)-3
            if(downsampled(i)>1.5*ex2)
                if(downsampled(i)>downsampled(i-3) && downsampled(i)>downsampled(i-2) && downsampled(i)>downsampled(i-1))
                    if (downsampled(i)>downsampled(i+1) && downsampled(i)>downsampled(i+2) && downsampled(i)>downsampled(i+3))
                        count = count +1;
                    end
                end
            end
        end

        bps(j+1,1) = count;
        jsec(j+1,1) = j;
        maxAmp(j+1,1) = max(fullwaverect);
        % if silent completely
        if (maxAmp(j+1,1) == 0)
            maxAmp(j+1,1) = -10;
        end
    end
    bps = round((bps/max(bps))*10);
    
    maxAmp = round(maxAmp/max(maxAmp)*4);
    output = [jsec, bps];
    
    filenametowrite = 'output.txt';
    fileID = fopen(filenametowrite,'w');
    filenametowrite2 = 'amplitudes.txt';
    fileID2 = fopen(filenametowrite2,'w');    
    for j = 0:length(output(:,1))-1
        fprintf(fileID, [num2str(output(j+1,2)), '\r\n']);
        fprintf(fileID2, [num2str(maxAmp(j+1)), '\r\n']);
    end    
    fclose(fileID);
    fclose(fileID2);
    
end
