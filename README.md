Telefonról vezérelhető karácsonyi díszvilágítás
=============================================

Történet
--------
A városban hagyomány, hogy karácsonykor élő kalendáriumot szerveznek. A lakosok közt szétosztják december napjait és az ablakot feldíszítik úgy, hogy az adott naptári nap száma megjelenik az ablakon. A miénk volt a kilences.

Kitaláltam, hogy fényeket teszek fel az ablakba, amit arduinoval fogok vezérelni. Aztán hirtelen eszembe jutott, hogy valahol az interneten láttam már hasonlót, hogy a fényeket bárki vezérelhette a telefonjáról. Így el kezdtem gondolkodni azon, hogy tudnám ezt megoldani.

Szerencsére minden szükséges alkatrészt megtaláltam itthon, így el kezdtem megépíteni.

Megvalósítás
-----------
Fényeknek a WS2812B címezhető ledszalagot használtam, mert így tudtam különböző animációkat létrehozni rajta. Minden egyes ledjét külön lehet vezérelni.

Volt bekötve egy relé is, mert az ablak elé feltett 220v-os fényfüzért is szerettem volna kapcsolhatóvá tenni.

Ezt az egészet egy NodeMCU vezérelte, ami csatlakozott egy külön (internetre nem csatlakoztatott) routerre.

A kint az utcára nézve ki volt rakva egy QR kód, amit ha a látogató beolvasott a telefonjával, megjelent a telefonon a vezérlő panel és irányíthatta a fényeket.
