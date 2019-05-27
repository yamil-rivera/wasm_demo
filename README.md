# Vividas Core

## Dependencies

```bash
$ brew install openssl
```

## Generating project/build files

```bash
mkdir build-darwin
cd build-darwin
cmake -G Xcode ..
xcodebuild -target viv_macos_player -configuration RelWithDebInfo
```
