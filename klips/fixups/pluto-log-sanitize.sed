s/^pluto\[[0-9]*\]: /pluto[PID]: /
s=^\(pluto\[PID\]: Starting Pluto (Openswan Version \).*)=\1VERSION)=
/Using KLIPS IPsec interface code/d
s/started helper pid=.* (fd:.*)/started helper pid=999 (fd:9)/
s/Plutorun started on .*/Plutorun started on DATE/
s/@(#) built on .* .* .* by .*@.*/@(#) built on DATE by PERSON/
s/\(received Delete SA\)(0x.*)\( payload: deleting IPSEC State #\).*/\1(0xSPI1SPI1)\2NUM/

