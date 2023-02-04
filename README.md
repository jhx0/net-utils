# net-utils

A collection of networking related tools that are small and simple to use.

## Usage
1. Clone/download this repository
2. Unpack/cd into the directory
3. Run
```Sh
$ make
```
4. Done

## List of programs
- **dns**: Fetch the IP address of the given site (A Record).
- **getip**: Fetches your public IP address.
- **header**: Fetches the HTTP header of a given site by issueing a GET request.

## Sample output
- **dns**: ./dns freebsd.org
```Sh
96.47.72.84
```
- **getip**: ./getip
```Sh
217.x.x.x
```
- **header**: ./header www.slackware.com
```Sh
HTTP/1.0 400 Bad Request
Server: AkamaiGHost
Mime-Version: 1.0
Content-Type: text/html
Content-Length: 208
Expires: Sat, 04 Feb 2023 13:46:46 GMT
Date: Sat, 04 Feb 2023 13:46:46 GMT
Connection: close
```

## Hacking
The tools should be kept small and easy to maintain. In general, one program should **do only one thing** - and that **well**!   
Functions that get called often should find there way into **util.h** so that all programs can benefit from them.

## Note
Currently, these tools target **Linux**. **GCC** and **Clang** both will build fine.
