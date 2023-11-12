import socket
import dnslib
import argparse
import time

# The server address and port.
SERVER_ADDRESS = "127.0.0.2"
SERVER_PORT = 12345

# The public DNS server.
PUBLIC_DNS = "223.5.5.5"

# The root DNS server address and port.
ROOT_DNS_ADDRESS = "193.0.14.129"
DNS_PORT = 53

# "domain name, class, type": ([(record, TTL)...], setting_time)
CACHE = {}


def public_dns_query(client_query):
    try:
        public_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        public_socket.settimeout(5)
        public_socket.sendto(client_query, (PUBLIC_DNS, DNS_PORT))
        response, public_address = public_socket.recvfrom(1024)
        d = dnslib.DNSRecord.parse(response)
        print("Received response from public DNS: \n", d)
        return response

    except socket.timeout:
        raise RuntimeError("Timeout")


def dns_query(client_query, server):
    try:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_socket.settimeout(5)
        server_socket.sendto(client_query, (server, DNS_PORT))
        response, _ = server_socket.recvfrom(1024)
        d = dnslib.DNSRecord.parse(response)
        print(f"Received response from {server}: \n", d)

        rr = d.rr
        auth = d.auth
        ar = d.ar

        new_rr = []
        for r in rr:
            if ":" in str(r.rdata):
                continue
            else:
                new_rr.append(r)

        new_auth = []
        for a in auth:
            if ":" in str(a.rdata):
                continue
            else:
                new_auth.append(a)

        new_ar = []
        for a in ar:
            if ":" in str(a.rdata):
                continue
            else:
                new_ar.append(a)

        result = dnslib.DNSRecord(header=d.header, q=d.q, rr=new_rr, auth=new_auth, ar=new_ar)
        return result.pack()

    except socket.timeout:
        raise RuntimeError("Timeout")


def iter_query(client_query, server_address):
    while True:
        # Query the root DNS server.
        response = dns_query(client_query, server_address)
        d = dnslib.DNSRecord.parse(response)

        # if answer given or no additional, return
        if d.rr or (d.auth and len(d.ar) < 2):

            # Found the IP address.
            print("Found the designated address.", [str(rr.rdata) for rr in d.rr])

            response = dnslib.DNSRecord(q=d.q, header=d.header).reply()

            if len(d.rr) != 0 and d.rr[0].rtype == dnslib.QTYPE.CNAME:
                print("CNAME record found. Continue searching.")

                cn_query = dnslib.DNSRecord.question(str(d.rr[0].rdata))
                print("New query: \n", cn_query)

                cn_res = iter_query(cn_query.pack(), ROOT_DNS_ADDRESS)
                rd = dnslib.DNSRecord.parse(cn_res)

                response.add_answer(*d.rr, *rd.rr)

            elif d.auth and len(d.ar) < 2 and len(d.rr) == 0:
                print("Authoritative server found, but no additional info. Continue searching.")

                auth_query = dnslib.DNSRecord.question(str(d.auth[0].rdata))
                print("New query: \n", auth_query)

                add_info_res = iter_query(auth_query.pack(), ROOT_DNS_ADDRESS)
                rd = dnslib.DNSRecord.parse(add_info_res)

                print()
                print("Continue searching.")
                server_address = str(rd.rr[0].rdata)
                print("New server address: ", server_address)
                print()
                continue

            else:
                response.add_answer(*d.rr)

            query = d.q
            CACHE[(str(query.qname), str(query.qclass), str(query.qtype))] = (
                response.rr, time.monotonic())

            return response.pack()

        else:
            # Continue searching.
            print()
            print("Continue searching.")
            server_address = str(d.ar[0].rdata)
            print("New server address: ", server_address)
            print()


def check_cache(query):
    query_key = (str(query.qname), str(query.qclass), str(query.qtype))
    if query_key in CACHE.keys():
        print("Found in cache.")
        records, setting_time = CACHE[query_key]
        for record in records:
            if time.monotonic() - setting_time < record.ttl:
                print("The record is still valid.")
            else:
                print("The record is expired.")
                del CACHE[query_key]
                return None
        return records
    else:
        print("Not found in cache.")
        return None


def main():
    assert FLAG == 0 or FLAG == 1, "FLAG must be 0 or 1."

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind((SERVER_ADDRESS, SERVER_PORT))
    print("The server is running in {} mode.".format(
        "public DNS" if FLAG == 0 else "iterative searching"))

    while True:
        client_query, client_address = server_socket.recvfrom(1024)
        d = dnslib.DNSRecord.parse(client_query)
        print("\n", "=" * 20, "Starting New Query", "=" * 20)
        print("Received query from client: \n", d)
        print()

        # Check the cache.
        record = check_cache(d.q)
        if record:
            response = dnslib.DNSRecord(q=d.q, header=d.header).reply()
            response.add_answer(*record)
            server_socket.sendto(response.pack(), client_address)
            continue

        if FLAG == 0:
            # Ask the public server for the IP address.
            response = public_dns_query(client_query)

            query = d.q
            CACHE[(str(query.qname), str(query.qclass), str(query.qtype))] = (
                d.rr, time.monotonic())
            server_socket.sendto(response, client_address)

        elif FLAG == 1:
            # Do the iterative searching.
            response = iter_query(client_query, ROOT_DNS_ADDRESS)
            server_socket.sendto(response, client_address)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-f",
        "-F",
        "--flag",
        type=int,
        default=0,
        help="0 for asking the public server for the IP address. "
             "1 for doing the iterative searching."
    )
    args = parser.parse_args()
    FLAG = args.flag
    main()
