function example_focusin
T = timer('timerfcn',@updatePlot,'period',0.05,'executionmode','fixedrate','taskstoexecute',500);
figure(1);
figure(2);
h = plot(rand(1,10));
ddata = 1:10;
i = 0;
start(T);
    function updatePlot(src,evt)
        set(h,'ydata',ddata+i);
        i = i+1;
    end
end