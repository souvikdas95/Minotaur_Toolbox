function startGateway()

	path_builder = get_absolute_file_path("builder_gateway_cpp.sce");

	third_party_dir = path_builder + filesep() + ".." + filesep() + ".." + filesep() + "thirdparty";

	if isdef("TOOLBOX_NAME") then
		LIBRARY_NAME = TOOLBOX_NAME + "_cpp";
	else
		LIBRARY_NAME = "Minotaur_Toolbox_cpp";
	end

	FUNC_NAMES = [
					"solve_qcqpglob","sci_qcqpglob"
				];

	FILES = [
				"sci_iofunc.cpp",
				"sci_qcqpglob.cpp"
			];

	[a, opt] = getversion();
	Version = opt(2);

	if (getos() == "Windows") then
		INC_DIR = [third_party_dir + filesep() + "Windows" + filesep() + "include"];
		LIB_DIR = [third_party_dir + filesep() + "Windows" + filesep() + Version + filesep() + "lib"];

		MINOTAUR_INC_DIR = [INC_DIR + filesep() + "minotaur"];

		LIB_COINUTILS = [LIB_DIR + filesep() + "libCoinUtils"];
		LIB_OSI = [LIB_DIR + filesep() + "libOsi"];
		LIB_CLP = [LIB_DIR + filesep() + "libClp"];
		LIB_OSICLP = [LIB_DIR + filesep() + "libOsiClp"];
		LIB_CLPSOLVER = [LIB_DIR + filesep() + "libClpSolver"];
		LIB_COINMUMPS = [LIB_DIR + filesep() + "libcoinmumps"];
		LIB_IPOPT = [LIB_DIR + filesep() + "libipopt"];
		LIB_MINOTAUR = [LIB_DIR + filesep() + "libminotaur"];
		LIB_MNTROSILP = [LIB_DIR + filesep() + "libmntrosilp"];
		LIB_MNTRIPOPT = [LIB_DIR + filesep() + "libmntripopt"];
		LIB_MNTRENGFAC = [LIB_DIR + filesep() + "libmntrengfac"];

		CXX_FLAGS = ["-D__USE_DEPRECATED_STACK_FUNCTIONS__ -w -fpermissive -I""" + path_builder + """ -I""" + INC_DIR + """ -I""" + MINOTAUR_INC_DIR + """ -Wl,-rpath=""" + LIB_DIR + """ "];
		LD_FLAGS = ["-L""" + LIB_COINUTILS + """ -L""" + LIB_OSI + """ -L""" + LIB_CLP + """ -L""" + LIB_OSICLP + """ -L""" + LIB_CLPSOLVER + """ -L""" + LIB_COINMUMPS + """ -L""" + LIB_IPOPT + """ -L""" + LIB_MINOTAUR + """ -L""" + LIB_MNTROSILP + """ -L""" + LIB_MNTRIPOPT + """ -L""" + LIB_MNTRENGFAC + """ "];
	else
		INC_DIR = [third_party_dir + filesep() + "linux" + filesep() + "include"];
		LIB_DIR = [third_party_dir + filesep() + "linux" + filesep() + Version + filesep() + "lib"];

		MINOTAUR_INC_DIR = [INC_DIR + filesep() + "minotaur"];

		LIB_COINUTILS = [LIB_DIR + filesep() + "libCoinUtils"];
		LIB_OSI = [LIB_DIR + filesep() + "libOsi"];
		LIB_CLP = [LIB_DIR + filesep() + "libClp"];
		LIB_OSICLP = [LIB_DIR + filesep() + "libOsiClp"];
		LIB_CLPSOLVER = [LIB_DIR + filesep() + "libClpSolver"];
		LIB_COINMUMPS = [LIB_DIR + filesep() + "libcoinmumps"];
		LIB_IPOPT = [LIB_DIR + filesep() + "libipopt"];
		LIB_MINOTAUR = [LIB_DIR + filesep() + "libminotaur"];
		LIB_MNTROSILP = [LIB_DIR + filesep() + "libmntrosilp"];
		LIB_MNTRIPOPT = [LIB_DIR + filesep() + "libmntripopt"];
		LIB_MNTRENGFAC = [LIB_DIR + filesep() + "libmntrengfac"];

		CXX_FLAGS = ["-D__USE_DEPRECATED_STACK_FUNCTIONS__ -w -fpermissive -I""" + path_builder + """ -I""" + INC_DIR + """ -I""" + MINOTAUR_INC_DIR + """ -Wl,-rpath=""" + LIB_DIR + """ "];
		LD_FLAGS = ["-L""" + LIB_COINUTILS + """ -L""" + LIB_OSI + """ -L""" + LIB_CLP + """ -L""" + LIB_OSICLP + """ -L""" + LIB_CLPSOLVER + """ -L""" + LIB_COINMUMPS + """ -L""" + LIB_IPOPT + """ -L""" + LIB_MINOTAUR + """ -L""" + LIB_MNTROSILP + """ -L""" + LIB_MNTRIPOPT + """ -L""" + LIB_MNTRENGFAC + """ "];
	end

	tbx_build_gateway(LIBRARY_NAME, FUNC_NAMES, FILES, path_builder, [], LD_FLAGS, CXX_FLAGS);

endfunction

startGateway();

clear startGateway;
