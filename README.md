# ps5-payload-sdk

## This fork (olegon85/sdk)

Carries evidence-based additions on top of upstream while they wait for
upstream review (see ps5-payload-dev/sdk#84). Every declaration here marked
"measured" comes from a payload process run against real hardware
(PS5 Pro, `kernel_get_fw_version` = `0x13600007`, loaded through elfldr),
with the probe kept as a runnable sample:

| Header | Sample | Measured on 13.60 |
|---|---|---|
| `include/ps5/pthread.h` | `samples/hello_pthread` | cond/mutex/rwlock init-destroy round trip, one-word caller slots, `pthread_cond_setname_np` name-once rule (EAGAIN when already named), NULL cond faults |
| `include/ps5/cescs.h` | `samples/hello_cescs` | 47 single-byte tables byte-for-byte against public sources, ISO 8859-1 identity, error codes and output-zeroing rules |
| `include/ps5/rtc.h` | `samples/hello_rtc` | tick epoch year 1, zone limits, day-of-week order of checks, 16-bit year wrap, negative-tick asymmetry |
| (no header: dlsym by mangled name) | `samples/hello_abstract` | the eight constant answers of `libSceAbstractLocal.sprx`: 0, `0x37C5A`, `0x809E0002`, `0x809E0005` reproduced unchanged |

### Resolving exports that have no name string

The export tables of retail modules store symbols by NID hash; the name string is optional and most unnamed exports carry none. Passing the **NID text itself** to `dlsym` resolves them (observed on a PS5 Pro at firmware 13.60, payload process loaded through elfldr; mechanism not investigated):

```c
void* lib = dlopen("libScePsm.sprx", RTLD_LAZY);
void* p = dlsym(lib, "PN1KQl3wnoU"); /* -> resolves; the call returns 0x80B30006 */
```

Measured with this technique: the nine unnamed `libkernel_sys` exports (`pCbBddNl66Y` and friends), three of `libScePsm`, `Vf6lJz4-bEU` in `libwebrtc`, `aFrwE9W9faU` in `libSceCdlgUtilServer` and the three NIDs of `libSceFsInternalForVsh`. It also proves absence: `libSceFreeTypeOt.sprx` resolves `VW4GD-TaIUc` (returns 7) but not `sceFreeTypeOtLibrarySetLcdFilter` - that name string exists nowhere in its table.

Firmware-specific knowledge only lands here when a sample reproduces it on
hardware; API signatures follow the PS4/PS5 SDK families until measured
otherwise. To use this fork as your SDK: install the upstream release zip,
then copy `include/ps5/*.h` over its `target/include/ps5/`.
This is an SDK for developing payloads targeted at exploited PS5s. ELF loaders
known to work include:
- [ps5-payload-elfldr][elfldr]
- [ps5-payload-websrv][websrv]
- [bdj-ipv6-hen][bdj-ipv6-hen]
- [elfloader][elfloader] via [ps5-jar-loader]
- [remote_lua_loader][remote_lua_loader]

Several artifacts in this repository originate from the [PS5SDK][PS5SDK] project.

## Prerequisites
On Debian-flavored operating systems, you can invoke the following commands to
install dependencies used by the SDK.
```console
john@localhost:ps5-payload-dev/sdk$ sudo apt-get update && sudo apt-get upgrade # optional
john@localhost:ps5-payload-dev/sdk$ sudo apt-get install bash clang-18 lld-18 wget # required
john@localhost:ps5-payload-dev/sdk$ sudo apt-get install socat cmake meson pkg-config python3 python3-pyelftools # optional
```

If you are using Fedora, you can install dependencies as follows (tested with version 41):
```console
john@localhost:ps5-payload-dev/sdk$ sudo dnf install bash llvm-devel clang lld wget # required
john@localhost:ps5-payload-dev/sdk$ sudo dnf install socat cmake meson pkg-config python3 python3-pyelftools # optional
```

If you are using macOS, you can install them using the [Homebrew Package Manager][macos-brew] (tested with macOS Sequoia):
```console
john@localhost:ps5-payload-dev/sdk$ brew install llvm@18 wget # required
john@localhost:ps5-payload-dev/sdk$ export LLVM_CONFIG=/opt/homebrew/opt/llvm@18/bin/llvm-config # required
john@localhost:ps5-payload-dev/sdk$ brew install socat cmake meson python && pip3 install pyelftools # optional
```

## Quick-start
You can download a binary distribution of the SDK from [the latest release page][latest-rel],
then install it to your local storage, e.g,
```console
john@localhost:tmp$ wget https://github.com/ps5-payload-dev/sdk/releases/latest/download/ps5-payload-sdk.zip
john@localhost:tmp$ sudo unzip -d /opt ps5-payload-sdk.zip
```
Assuming you have all the prerequisites and you are on a POSIX system,
the binary distribution should work regardless of CPU architecture, e.g., x86_64, aarch64.

## Usage
```console
john@localhost:ps5-payload-dev/sdk$ export PS5_PAYLOAD_SDK=/opt/ps5-payload-sdk
john@localhost:ps5-payload-dev/sdk$ make -C samples/hello_world
john@localhost:ps5-payload-dev/sdk$ export PS5_HOST=ps5; export PS5_PORT=9021
john@localhost:ps5-payload-dev/sdk$ make -C samples/hello_world test
```

## Building the SDK
```console
john@localhost:ps5-payload-dev/sdk$ sudo make DESTDIR=/opt/ps5-payload-sdk install
john@localhost:ps5-payload-dev/sdk$ export PS5_PAYLOAD_SDK=/opt/ps5-payload-sdk
john@localhost:ps5-payload-dev/sdk$ sudo -E ./libcxx.sh # fetch, build, and install libcxx
```

## Adding new SCE Libs
If you have decrypted sprx files that you would like to interact with, you can
build stubs for them as follows (Make sure you have already installed the optional dependencies for your OS):
```console
john@localhost:ps5-payload-dev/sdk$ ln -s /path/to/sprx/libSceXYZ.sprx sce_stubs/libSceXYZ.sprx
john@localhost:ps5-payload-dev/sdk$ make -C sce_stubs stubs
john@localhost:ps5-payload-dev/sdk$ sudo make DESTDIR=/opt/ps5-payload-sdk install
```

## Reporting Bugs
If you encounter problems with the SDK, please [file a github issue][issues].
If you plan on sending pull requests which affect more than a few lines of code,
please file an issue before you start to work on you changes. This will allow us
to discuss the solution properly before you commit time and effort.

## License
Files in the folder include/freebsd are licenced under BSD licences.
Unless otherwhise explicitly stated inside a file, the rest are licensed under
the GPLv3+.

[issues]: https://github.com/ps5-payload-dev/sdk/issues/new
[latest-rel]: https://github.com/ps5-payload-dev/sdk/releases/latest
[elfldr]: https://github.com/ps5-payload-dev/elfldr
[websrv]: https://github.com/ps5-payload-dev/websrv
[bdj-ipv6-hen]: https://github.com/ps5-payload-dev/bdj-ipv6-hen
[remote_lua_loader]: https://github.com/shahrilnet/remote_lua_loader
[elfloader]: https://github.com/cryonumb/elfloader
[ps5-jar-loader]: https://github.com/hammer-83/ps5-jar-loader
[PS5SDK]: https://github.com/PS5Dev/PS5SDK
[macos-brew]: https://brew.sh
