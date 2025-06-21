#!/bin/bash

HOST="$1"
PORT="$2"
PASS="$3"
DELAY=1    # Delay between commands (seconds)
TIMEOUT=5  # Timeout for each nc session in seconds

if [ -z "$HOST" ] || [ -z "$PORT" ] || [ -z "$PASS" ]; then
  echo "Usage: $0 <host> <port> <password>"
  exit 1
fi

log() {
  echo "[$(date '+%T')] $*"
}

send_cmds() {
  local test_func="$1"
  local test_name="$2"

  log "Connecting to $HOST:$PORT for '$test_name'..."

  # Capture server output for this test
  local outfile="irc_test_${test_name}.log"
  rm -f "$outfile"

  # Start nc to read server responses in background
  timeout $TIMEOUT nc "$HOST" "$PORT" | tee "$outfile" | while IFS= read -r line; do
    echo "[SERVER] $line"
  done &
  nc_pid=$!

  # Open tcp connection for writing commands
  exec 3<>/dev/tcp/"$HOST"/"$PORT" || { log "Failed to open connection"; kill $nc_pid; return 1; }

  # Run test function outputting commands to fd 3
  $test_func >&3

  # Close write fd so server sees EOF
  exec 3>&-

  # Wait for nc reader to finish
  wait $nc_pid

  log "Connection closed for '$test_name'."
}

# ---------------------- Test Cases ----------------------

test_connect_and_register() {
  log "=== Test: Correct Connect and Register ==="
  echo "PASS $PASS"
  sleep $DELAY
  echo "NICK testuser"
  sleep $DELAY
  echo "USER testuser 0 * :Test User"
  sleep $DELAY
  echo "QUIT :Bye"
}

test_bad_password() {
  log "=== Test: Bad Password ==="
  echo "PASS wrongpass"
  sleep $DELAY
  echo "NICK baduser"
  sleep $DELAY
  echo "USER baduser 0 * :Bad User"
  sleep $DELAY
  echo "QUIT :Bye"
}

test_join_and_privmsg() {
  log "=== Test: Join Channel and Send Message ==="
  echo "PASS $PASS"
  sleep $DELAY
  echo "NICK joiner"
  sleep $DELAY
  echo "USER joiner 0 * :Joiner User"
  sleep $DELAY
  echo "JOIN #testchannel"
  sleep $DELAY
  echo "PRIVMSG #testchannel :Hello channel!"
  sleep $DELAY
  echo "QUIT :Bye"
}

test_operator_commands() {
  log "=== Test: Operator Commands ==="
  echo "PASS $PASS"
  sleep $DELAY
  echo "NICK operuser"
  sleep $DELAY
  echo "USER operuser 0 * :Operator User"
  sleep $DELAY
  echo "JOIN #opchannel"
  sleep $DELAY
  echo "MODE #opchannel +o operuser"
  sleep $DELAY
  echo "TOPIC #opchannel :New topic set by operator"
  sleep $DELAY
  echo "INVITE joiner #opchannel"
  sleep $DELAY
  echo "KICK #opchannel joiner :You are kicked!"
  sleep $DELAY
  echo "QUIT :Bye"
}

test_partial_messages() {
  log "=== Test: Partial Message Sending ==="
  echo "PASS $PASS"
  sleep 0.3
  echo -n "NI"
  sleep 0.3
  echo "CK partialuser"
  sleep 0.3
  echo -n "USE"
  sleep 0.3
  echo "R partialuser 0 * :Partial User"
  sleep 0.3
  echo "JOIN #partial"
  sleep 0.3
  echo "PRIVMSG #partial :Hello with partial messages!"
  sleep 0.3
  echo "QUIT :Bye"
}

test_no_nick() {
  log "=== Test: Register Without NICK ==="
  echo "PASS $PASS"
  sleep $DELAY
  echo "USER nonick 0 * :No Nickname"
  sleep $DELAY
  echo "QUIT :Bye"
}

test_no_user() {
  log "=== Test: Register Without USER ==="
  echo "PASS $PASS"
  sleep $DELAY
  echo "NICK nouser"
  sleep $DELAY
  echo "QUIT :Bye"
}

test_invalid_command() {
  log "=== Test: Invalid Command ==="
  echo "PASS $PASS"
  sleep $DELAY
  echo "NICK badcmd"
  sleep $DELAY
  echo "USER badcmd 0 * :Bad Cmd"
  sleep $DELAY
  echo "FOOBAR"
  sleep $DELAY
  echo "QUIT :Bye"
}

test_long_nick() {
  log "=== Test: Long Nickname ==="
  echo "PASS $PASS"
  sleep $DELAY
  echo "NICK veryveryverylongnickname"
  sleep $DELAY
  echo "USER longnick 0 * :Long Nick"
  sleep $DELAY
  echo "QUIT :Bye"
}

test_join_invalid_channel() {
  log "=== Test: Join Invalid Channel ==="
  echo "PASS $PASS"
  sleep $DELAY
  echo "NICK invchan"
  sleep $DELAY
  echo "USER invchan 0 * :Invalid Channel"
  sleep $DELAY
  echo "JOIN notachannel"
  sleep $DELAY
  echo "QUIT :Bye"
}

test_privmsg_without_join() {
  log "=== Test: PRIVMSG Without JOIN ==="
  echo "PASS $PASS"
  sleep $DELAY
  echo "NICK pmnojoin"
  sleep $DELAY
  echo "USER pmnojoin 0 * :PM No Join"
  sleep $DELAY
  echo "PRIVMSG #testchannel :Should not be allowed"
  sleep $DELAY
  echo "QUIT :Bye"
}

# ---------------------- Test Runner ----------------------

declare -a tests=(
  "test_connect_and_register"
  "test_bad_password"
  "test_join_and_privmsg"
  "test_operator_commands"
  "test_partial_messages"
  "test_no_nick"
  "test_no_user"
  "test_invalid_command"
  "test_long_nick"
  "test_join_invalid_channel"
  "test_privmsg_without_join"
)

log "Starting IRC server tests against $HOST:$PORT"
failures=0

for test_func in "${tests[@]}"; do
  send_cmds "$test_func" "$test_func"
  status=$?
  if [ $status -ne 0 ]; then
    log ">>> FAIL: $test_func"
    failures=$((failures+1))
  else
    log ">>> PASS: $test_func"
  fi
  sleep 1
done

if [ $failures -eq 0 ]; then
  log "All tests passed!"
else
  log "$failures test(s) failed. Check logs for details."
fi

log "Tests finished."

