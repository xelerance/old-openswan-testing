: ==== start ====
TESTNAME=ipv6-v4-through-v6-klips-klips
source /testing/pluto/bin/westlocal.sh

# confirm that the network is alive
ping -n -c 4 -I 192.0.1.254 192.0.2.254
# make sure that clear text does not get through
iptables -A INPUT -i eth1 -p icmp -j DROP
# confirm with a ping to east-in
ping -n -c 4 -I 192.0.1.254 192.0.2.254

ipsec setup start
/testing/pluto/bin/wait-until-pluto-started

# Temporary disabled and workaround for broken parser
# ipsec auto --add west-east-4in6
ipsec whack --name west-east-4in6 --ipv6 --tunnelipv4 --id @west --host 2001:db8:1:2::45 --client 192.0.1.0/24 --to --id @east --host 2001:db8:1:2::23 --client 192.0.2.0/2 --tunnel --psk --encrypt --pfs 
echo '@west @east: PSK "test"' >> /tmp/$TESTNAME/ipsec.secrets
ipsec secrets
# end Temp

ipsec whack --debug-control --debug-controlmore --debug-parsing --debug-crypt

echo done

