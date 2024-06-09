#!/bin/bash

HOST=$1
PORT=$2
USERNAME="Anonymous"
PASS=""

get_reply_code() {
    local reply=""
    local code=""
    while read -r -t 3 line <&3; do
        if [[ -n $line ]]; then 
            code=$(echo "$line" | grep -oP '^\d{3}')
            if [[ -n $code ]]; then
                echo "$code"
                return 
            fi
        fi
    done
    echo "$code"
}

launch_client() {
    exec 3<>/dev/tcp/$HOST/$PORT
    local response=""
    while read -r -t 3 line <&3; do
        response+="$line"
        if [[ -z $line ]]; then break; fi
    done

    if [[ "$response" == *"220"* ]]; then
        echo -e "waiting for connection 220 -- \033[1;32mOK\033[0m"
        return 0  # Indicate success with standard Unix convention
    else
        echo -e "waiting for connection 220 -- \033[1;31mKO\033[0m"
        return 1  # Indicate failure
    fi
}

kill_client() {
    echo "Closing connection..."
    exec 3>&-
}

print_succeeded() {
    local test_name=$1
    echo -e "$test_name: \033[1;32mSUCCESS\033[0m"
}

print_failure() {
    local test_name=$1
    echo -e "$test_name: \033[1;31mFAILURE\033[0m"
}

launch_test() {
    local test_name=$1
    local cmd=$2
    local expected_code=$3

    echo "Sent: $cmd"
    echo -e "$cmd\r" >&3

    local received_code=$(get_reply_code)

    if [[ "$received_code" == "$expected_code" ]]; then
        echo -e "Got [\e[0;32m$received_code\033[0m] -- \033[1;32mOK\033[0m"
    else
        echo -e "Got [\e[0;31m$received_code\033[0m] Expected $expected_code -- \033[1;31mKO\033[0m"
        kill_client
        return 1
    fi
}

test_auth() {
    local test_name="Authentication"

    echo -e "\e[1;36m-----[$test_name]-----\033[0m"
    launch_client
    if [[ $? -ne 0 ]]; then
        echo "Connection failed to meet initial requirements."
        print_failure "$test_name"
        return
    fi

    local cmd1="USER $USERNAME"
    local cmd2="PASS $PASS"
    local expected_codes1=331
    local expected_codes2=230
    launch_test "$test_name" "$cmd1" 331 || { print_failure "$test_name"; return; }
    launch_test "$test_name" "$cmd2" 230 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
}

test_noop() {
    local test_name="NOOP Testing"

    echo -e "\e[1;36m-----[$test_name]-----\033[0m"

    local cmd1="NOOP"

    launch_test "$test_name" "$cmd1" 200 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
}

test_help() {
    local test_name="HELP Testing"

    echo -e "\e[1;36m-----[$test_name]-----\033[0m"

    local cmd1="HELP"
    local cmd2="HELP NOOP"

    launch_test "$test_name" "$cmd1" 214 || { print_failure "$test_name"; return; }
    launch_test "$test_name" "$cmd2" 214 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
}

test_quit() {
    local test_name="QUIT Testing"

    echo -e "\e[1;36m-----[$test_name]-----\033[0m"

    local cmd1="QUIT"

    launch_test "$test_name" "$cmd1" 221 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
}

test_advanced_auth() {
    local test_name="ADVANCED AUTH Testing"

    echo -e "\e[1;36m-----[$test_name]-----\033[0m"
    launch_client
    local cmd1="PASS"
    local cmd2="USER test"
    local cmd3="USER $USERNAME"

    launch_test "$test_name" "$cmd1" 530 || { print_failure "$test_name"; return; }
    launch_test "$test_name" "$cmd2" 331 || { print_failure "$test_name"; return; }
    launch_test "$test_name" "$cmd1" 530 || { print_failure "$test_name"; return; }
    launch_test "$test_name" "$cmd3" 331 || { print_failure "$test_name"; return; }
    launch_test "$test_name" "$cmd1" 230 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
}

test_pwd() {
    local test_name="PWD Testing"

    echo -e "\e[1;36m-----[$test_name]-----\033[0m"

    local cmd1="PWD"

    launch_test "$test_name" "$cmd1" 257 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
}

test_cwd() {
    local test_name="CWD Testing"

    echo -e "\e[1;36m-----[$test_name]-----\033[0m"

    local cmd1="CWD .."

    launch_test "$test_name" "$cmd1" 250 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
}

test_cdup() {
    local test_name="CDUP Testing"

    echo -e "\e[1;36m-----[$test_name]-----\033[0m"

    local cmd1="CDUP"

    launch_test "$test_name" "$cmd1" 200 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
}

test_invalid_cmd() {
    local test_name="INVALID COMMAND Testing"

    echo -e "\e[1;36m-----[$test_name]-----\033[0m"

    local cmd1="test"
    local cmd2="          "

    launch_test "$test_name" "$cmd1" 500 || { print_failure "$test_name"; return; }
    launch_test "$test_name" "$cmd2" 500 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
}

test_pasv() {
    local test_name="PASV Testing"

    echo -e "\e[1;36m-----[$test_name]-----\033[0m"
    launch_client
    local cmd1="USER $USERNAME"
    local cmd2="PASS $PASS"
    local cmd3="PASV"

    launch_test "$test_name" "$cmd1" 331 || { print_failure "$test_name"; return; }
    launch_test "$test_name" "$cmd2" 230 || { print_failure "$test_name"; return; }
    launch_test "$test_name" "$cmd3" 227 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
}

test_port() {
    local test_name="PORT Testing"

    echo -e "\e[1;36m-----[$test_name]-----\033[0m"

    local cmd3="PORT"
    local cmd4="PORT 127,0,0,1,15,200"

    launch_test "$test_name" "$cmd3" 550 || { print_failure "$test_name"; return; }
    launch_test "$test_name" "$cmd4" 200 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
    kill_client
}

test_retr() {
    local test_name="RETR Testing"

    echo "$test_name testing example" >> outTest
    echo -e "\e[1;36m-----[$test_name]-----\033[0m"
    local cmd1="RETR outTest"

    launch_test "$test_name" "$cmd1" 150 || { print_failure "$test_name"; return; }
    print_succeeded "$test_name"
}

# Run the authentication test
test_auth
test_invalid_cmd
test_noop
test_help
test_quit
test_advanced_auth
test_pwd
test_cwd
test_cdup
test_port
test_pasv
test_retr
kill_client
