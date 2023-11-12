# README

Name: Haopeng Chen

ID: 120090645

## How to Start

```bash
# Make sure you have installed dnslib
# pip install dnslib

# Public DNS
python main.py
# or
python main.py -F 0

# Iterative Searching
python main.py -F 1

# Maybe also use python3 to substitute python on some machines
```

Then go to another window

```bash
dig www.example.com @127.0.0.2 -p 12345
dig www.baidu.com @127.0.0.2 -p 12345

# Test cache, should be return in 0ms
dig www.example.com @127.0.0.2 -p 12345

# You could also change listening address and port in source code.
# Cache will expire in cordance of TTL, you could also find a server to test it.
```

**Expected returns, respectively.**

*Note: Pay attention to the python program window, as some IP address returned by the root server and TLDs could not be reached, which leads to the* `Timeout Error`.

```bash
; <<>> DiG 9.18.19-1~deb12u1-Debian <<>> www.example.com @127.0.0.2 -p 12345
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 11671
;; flags: qr aa rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 0

;; QUESTION SECTION:
;www.example.com.               IN      A

;; ANSWER SECTION:
www.example.com.        86400   IN      A       93.184.216.34

;; Query time: 1219 msec
;; SERVER: 127.0.0.2#12345(127.0.0.2) (UDP)
;; WHEN: Sun Oct 15 16:33:56 CST 2023
;; MSG SIZE  rcvd: 49
```

```bash
; <<>> DiG 9.18.19-1~deb12u1-Debian <<>> www.baidu.com @127.0.0.2 -p 12345
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 25065
;; flags: qr aa rd ra; QUERY: 1, ANSWER: 3, AUTHORITY: 0, ADDITIONAL: 0

;; QUESTION SECTION:
;www.baidu.com.                 IN      A

;; ANSWER SECTION:
www.baidu.com.          1200    IN      CNAME   www.a.shifen.com.
www.a.shifen.com.       120     IN      A       157.148.69.74
www.a.shifen.com.       120     IN      A       157.148.69.80

;; Query time: 709 msec
;; SERVER: 127.0.0.2#12345(127.0.0.2) (UDP)
;; WHEN: Sun Oct 15 16:34:37 CST 2023
;; MSG SIZE  rcvd: 90
```

```bash
; <<>> DiG 9.18.19-1~deb12u1-Debian <<>> www.example.com @127.0.0.2 -p 12345
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 47940
;; flags: qr aa rd ra ad; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 0

;; QUESTION SECTION:
;www.example.com.               IN      A

;; ANSWER SECTION:
www.example.com.        86400   IN      A       93.184.216.34

;; Query time: 0 msec
;; SERVER: 127.0.0.2#12345(127.0.0.2) (UDP)
;; WHEN: Sun Oct 15 16:34:52 CST 2023
;; MSG SIZE  rcvd: 49
```

