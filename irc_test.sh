#!/bin/bash

HOST=localhost
PORT="$1"
PASS="$2"
DELAY=1  # Delay between commands (seconds)
TIMEOUT=5 # Timeout for each nc session in seconds

if [ -z "$HOST" ] || [ -z "$PORT" ] || [ -z "$PASS" ]; then
  echo "Usage: $0 <port> <password>"
  exit 1
fi

log() {
  echo "[$(date '+%T')] $*"
}

send_cmds() {
  local test_func="$1"

  log "Connecting to $HOST:$PORT..."

  timeout_pid=$(nc "$HOST" "$PORT" & echo $!)
  sleep $TIMEOUT && kill $timeout_pid

  if ! ps -p $nc_pid > /dev/null; then
    log "Failed to open connection"
    return 1
  fi

  # Open tcp connection for writing commands
  exec 3<>/dev/tcp/"$HOST"/"$PORT" || { log "Failed to open connection"; kill $nc_pid; return 1; }

  # Run test function outputting commands to fd 3
  $test_func >&3

  # Close write fd so server sees EOF
  exec 3>&-

  # Wait for nc reader to finish
  wait $nc_pid

  log "Connection closed."
}

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

log "Starting IRC server tests against $HOST:$PORT"

send_cmds test_connect_and_register
send_cmds test_bad_password
send_cmds test_join_and_privmsg
send_cmds test_operator_commands
send_cmds test_partial_messages

log "Tests finished."
