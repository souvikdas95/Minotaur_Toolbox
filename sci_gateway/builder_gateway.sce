sci_gateway_dir = get_absolute_file_path('builder_gateway.sce');

tbx_builder_gateway_lang('cpp', sci_gateway_dir);

tbx_build_gateway_loader(['cpp'], sci_gateway_dir);

clear sci_gateway_dir;
