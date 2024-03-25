# BESST

BESST is the name for both the official BAFANG diagnostic software and the associated software tool.

Included are Binaries for different BESST versions and the (Javascript) sourcecode residing inside the app.asar electron
application (the actuall BESST software sourcecode)

## Login Bypass

Included is a special bypass program to login to BESST with every credential you want and get unlimited access to the
tools.

It requires python3 and the following python (pip) packages: `socketserver`.

To run the script, edit your system hostfile (`C:\windows\system32\drivers\etc\hosts` for example) and point the
following domains to `127.0.0.1`: 

- `api.besst.bafang-service.com`
- `test.api.besst.bafang-service.com`
- `eu-central-1.api.besst.bafang-service.com`
- `bafang-besst.oss-cn-shanghai.aliyuncs.com`

On Windows that would look like:

```
# localhost name resolution is handled within DNS itself.
#	127.0.0.1       localhost
#	::1             localhost
127.0.0.1 api.besst.bafang-service.com
127.0.0.1 test.api.besst.bafang-service.com
127.0.0.1 eu-central-1.api.besst.bafang-service.com
127.0.0.1 bafang-besst.oss-cn-shanghai.aliyuncs.com
```