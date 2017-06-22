verboseMode = ilib_verbose();
ilib_verbose(0);

TOOLBOX_NAME = "Minotaur_Toolbox";
TOOLBOX_TITLE = "Minotaur Toolbox";

function main_builder()

    toolbox_dir = get_absolute_file_path("builder.sce");

    // Check Scilab version
    try
        v = getversion("scilab");
    catch
        error(gettext("Scilab 5.3 or more is required."));
    end
    if v(1) < 5 & v(2) < 3 then
        // new API in scilab 5.3
        error(gettext("Scilab 5.3 or more is required."));
    end

    // Check modules_manager module availability
    if ~isdef("tbx_build_loader") then
        error(msprintf(gettext("%s module not installed."), "modules_manager"));
    end

    // Build Macros, Source and Gateway
    tbx_builder_macros(toolbox_dir);
    // tbx_builder_src(toolbox_dir);
    tbx_builder_gateway(toolbox_dir);
    
    // Clear old Help & Demo files
	fmnames = mopen("macros" + filesep() + "names", "r");
	mnames = mgetl(fmnames);
	for i = 1:size(mnames, "r")
		mdelete("help" + filesep() + "en_US" + filesep() + mnames(i) + ".xml");
		mdelete("demos" + filesep() + mnames(i) + ".dem.sce");
	end
	mclose(fmnames);

	// Generate and Build Help
    help_from_sci("macros", "help" + filesep() + "en_US", "demos");
    tbx_builder_help(toolbox_dir);

    // Build Loader & Cleaner
    tbx_build_loader(TOOLBOX_NAME, toolbox_dir);
    tbx_build_cleaner(TOOLBOX_NAME, toolbox_dir);

endfunction

main_builder();

ilib_verbose(verboseMode);

clear main_builder, TOOLBOX_NAME, TOOLBOX_TITLE, verboseMode;
