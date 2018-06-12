# nef2tif

**nef2tif** is a simple Windows desktop application converting an image in Nikon's Raw image format NEF (Nikon Electronic Format) to TIFF.

Unlike most of the other free and open source raw image format decoders and converters, **nef2tif** uses Nikon's NEF(RAW) SDK, so that optimal image quality is ensured. 

The main purpose of this little program is to run on Linux with Wine.


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.



### Prerequisites

Go to [http://sdk.nikonimaging.com/apply/](http://sdk.nikonimaging.com/apply/) to download the *NEF(RAW) SDK*.
Unzip the downloaded file.

Download the source code of **nef2tif** via github:

```
git clone https://github.com/alexanderloewen/nef2tif.git
```

Make sure you have Microsoft Visual Studio 2017 installed.



### Installing

1. Open the solution *nef2tif.sln* within Visual Studio.
2. Select project *nef2tif* in the Solution Explorer.
3. Go to *Project->Manage NuGet Packages...* and install *libtiff*.
4. Copy the file `Image SDK\Library\win\Include\Nkfl_Interface.h` from Nikon's NEF(RAW) SDK to the project folder.
5. Add the following files to the project by right-clicking on the project and than `Add->Existing item...`:
   * `Nkfl_Interface.h`
   * `Image SDK\Library\win\Bin\x86\Release\Lib\NkImgSDK.lib`
   * `packages\libtiff.x.x.x.x\build\native\lib\Release\Win32\v140\libtiff.lib`
6. Go to `Project->nef2tif Properties...`, choose `C++` then `Code Generation`and change `Runtime Library` to `Multithreaded (/MT)` for better usage with wine. This makes a statically linked .exe.
7. Build the solution in the `Release/x86` configuration. (This is necessary because the 64-bit version of Nikon's DLL seems to be buggy.)



## Deployment

To run **nef2tif** make sure to have the following files in your PATH:
* `packages\libjpeg.redist.x.x.x.x\build\native\bin\v140\Win32\Release\dynamic\cdecl\jpeg.dll`
* `packages\libtiff.redist.x.x.x.x\build\native\bin\Release\Win32\v140\libtiff.dll`
* `packages\zlib.v140.windesktop.msvcstl.dyn.rt-dyn.x.x.x.x\lib\native\v140\windesktop\msvcstl\dyn\rt-dyn\Win32\Release\zlib.dll`



## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on my code of conduct, and the process for submitting pull requests to me.



## Authors

* **Alexander Löwen** - *Initial work* - [alexanderloewen](https://github.com/alexanderloewen)

See also the list of [contributors](contributors) who participated in this project.



## License

This project is licensed under the GNU General Public License 3 - see the [LICENSE.md](LICENSE.md) file for details.

