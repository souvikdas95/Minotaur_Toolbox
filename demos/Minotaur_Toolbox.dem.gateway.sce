function subdemolist = demo_gateway()

    demopath = get_absolute_file_path(TOOLBOX_NAME + ".dem.gateway.sce");

    subdemolist = ["qcqpglob", "qcqpglob.dem.sce"];

    subdemolist(:,2) = demopath + subdemolist(:,2);

endfunction

subdemolist = demo_gateway();

clear demo_gateway;
