SERVER="localhost"  
PORT=6667      
CHANNEL="#Room"
CLIENT_COUNT=100
PASS="pass"

for ((i=1; i<=CLIENT_COUNT; i++))
do
    (
    NICKNAME="TestUser_${i}"
    echo "PASS $PASS"
    sleep 0.5
    echo "NICK $NICKNAME"
    sleep 0.5
    echo "USER ${i} 0 * "
    sleep 0.5
    echo "JOIN $CHANNEL"
    sleep 200
    ) | nc $SERVER $PORT &
    sleep 2
done
wait 
echo "Disconnected all clients from the server."
PRIVMSG #Room :heeloe this me 
PRIVMSG Emet #:zbi skou
STOPTALKING #