# Informationen

- Wir haben im Firefox unter Windows und Linux getestet
- In [proxy.conf.json](frontend/src/proxy.conf.json) musste ich unter Linux localhost zu 127.0.0.1 ändern, damit der proxy funktioniert
- Wenn ein Element gelöscht werden soll, muss man die 3 Sekunden in der Liste, in welcher sich das Item befindet, bleiben. Bei einem Listenwechsel wird der Löschvorgang abgebrochen