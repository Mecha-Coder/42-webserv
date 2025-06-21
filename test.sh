#!/bin/bash
GREEN="\033[0;32m"
RED="\033[0;31m"
PURPLE="\033[0;35m"
YELLOW="\033[0;33m"
NC="\033[0m" # No Color

run_command() {
    INPUT="$1"
    echo -e "${PURPLE}Input: ${NC}"
    echo -e "$INPUT"

    #OUTPUT=$(./cub3D "$INPUT" 2>&1)
    OUTPUT=$(valgrind -q --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --error-exitcode=123 ./webserv "$INPUT" 2>&1)

    EXIT_STATUS=$?
    
    echo -e "${PURPLE}Output: ${NC}"
    echo -e "$OUTPUT"

    if [ $EXIT_STATUS -eq 123 ]; then
        echo -e "${PURPLE}Valgrind: ${RED}KO${NC}"
    else
        echo -e "${PURPLE}Valgrind: ${GREEN}OK${NC}"
    fi

    echo -e "${YELLOW}--------------------------------------------------------------------${NC}"
}
    echo -e "\n\n${YELLOW} Wrong configuration ${NC}"
    echo -e "${YELLOW} =================== ${NC}\n\n"

    run_command "./config/bad/array_no_bracket.toml"
	run_command "./config/bad/array_no_comma.toml"
	run_command "./config/bad/broadcast_ip.toml"
	run_command "./config/bad/bullshit_input.toml"
	run_command "./config/bad/clientBody_negative_num.toml"
	run_command "./config/bad/clientBody_not_num.toml"
	run_command "./config/bad/clientBody_superLarge.toml"
	run_command "./config/bad/duplicate_directive@location.toml"
	run_command "./config/bad/duplicate_directive@server.toml"
	run_command "./config/bad/empty.toml"
	run_command "./config/bad/input_all_array.toml"
	run_command "./config/bad/input_all_single.toml"
	run_command "./config/bad/input_array_to_single.toml"
	run_command "./config/bad/input_single_to_array.toml"
	run_command "./config/bad/invalid_bool_value.toml"
	run_command "./config/bad/invalid_cgi_extension.toml"
	run_command "./config/bad/invalid_directive@location.toml"
	run_command "./config/bad/invalid_directive@server.toml"
	run_command "./config/bad/invalid_errorPg_code.toml"
	run_command "./config/bad/invalid_errorPg_format.toml"
	run_command "./config/bad/invalid_ip.toml"
	run_command "./config/bad/invalid_port.toml"
	run_command "./config/bad/location_comes_first.toml"
	run_command "./config/bad/missing_equal.toml"
	run_command "./config/bad/missing_header@location.toml"
	run_command "./config/bad/missing_header@server.toml"
	run_command "./config/bad/missing_mandatory@location.toml"
	run_command "./config/bad/missing_mandatory@server.toml"
	run_command "./config/bad/missing_quote.toml"
	run_command "./config/bad/missing_values.toml"
	run_command "./config/bad/mixed_directive.toml"
	run_command "./config/bad/muti_directive_inline.toml"
	run_command "./config/bad/no_directive.toml"
	run_command "./config/bad/only_location.toml"
	run_command "./config/bad/weird_method.toml"
	#run_command "./config/bad/no_permission.toml"
	run_command "   "
	run_command "no_toml_ext.txt"
    
    #echo -e "\n\n${YELLOW} Correct configuration ${NC}"
    #echo -e "${YELLOW} ===================== ${NC}\n\n"

	#run_command "./config/good/bare_minimum.toml"
	#run_command "./config/good/config.toml"
	#run_command "./config/good/default_values.toml"
	#run_command "./config/good/eval.toml"
	#run_command "./config/good/inconsistent_space.toml"
	#run_command "./config/good/invalid_errorPg_path.toml"
	#run_command "./config/good/invalid_root_path.toml"
	#run_command "./config/good/makefiel_as_defaultPg.toml"
	#run_command "./config/good/no_location.toml"
	#run_command "./config/good/no_server_name.toml"
	#run_command "./config/good/original.toml"
	#run_command "./config/good/select_correct_location.toml"
	#run_command "./config/good/valid_bool_value.toml"	
	#run_command "./config/good/valid_errPg_format.toml"


	