# Build Instructions for UI

This is a small helper on how to setup your build environment for compiling and running the UI.
This tutorial is meant for Linux users running KDE.

> [!NOTE]
> Make sure to clone the repository with the `--recursive` flag to get all the submodules.
> 
> ```bash
> git clone --recursive https://github.com/Amaan1234567/KoolIntelligence
> ```  


# Getting Dependencies

## Step 1: get basic dependencies
We need cmake, qt6, gcc, g++, and kirigami.
Run the below command relevant to your distribution.

### Arch:
```bash
sudo pacman -S base-devel extra-cmake-modules cmake kirigami ki18n kcoreaddons breeze kiconthemes qt6-base qt6-declarative qqc2-desktop-style sdl2 baloo kpty qt5compat
```

### OpenSUSE:
```bash
sudo zypper install cmake kf6-extra-cmake-modules kf6-kirigami-devel kf6-ki18n-devel kf6-kcoreaddons-devel kf6-kiconthemes-devel qt6-base-devel qt6-declarative-devel qt6-quickcontrols2-devel kf6-qqc2-desktop-style SDL2-devel kf6-baloo-devel kf6-kpty-devel qt6-qt5compat-devel
```

### Fedora:
```bash
sudo dnf groupinstall "Development Tools" "Development Libraries"
sudo dnf install cmake extra-cmake-modules kf6-kirigami2-devel kf6-ki18n-devel kf6-kcoreaddons-devel kf6-kiconthemes-devel qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qtquickcontrols2-devel kf6-qqc2-desktop-style SDL2-devel kf6-baloo-devel kf6-kpty-devel qt6-qt5compat-devel
```

### Ubuntu: (DOES NOT WORK)

Note: This will not work as kubuntu has still not packaged KDE 6. Till then this is pointless and just there for my(Rahul) reference
```bash
sudo apt install kirigami2-dev qml-module-org-kde-i18n-localedata qt6-base-dev libkf5iconthemes-dev qtdeclarative5-dev libkf5i18n-dev qtquickcontrols2-5-dev qml-module-org-kde-qqc2desktopstyle libkf5coreaddons-dev libsdl2-dev
```
### For other distributions:

The instructions were taken from this link
https://develop.kde.org/docs/getting-started/kirigami/introduction-getting_started/

Please refer to the link for more information.

## Step 2: Install Cuda Toolkit

Install the [nvidia cuda toolkit](https://developer.nvidia.com/cuda-downloads) from the official website.
After installation, make sure that the `nvcc` compiler is in your path.

## Step 3: Install GCC-12
Since nvcc cannot use latest versions of gcc, we need to install gcc-12.
We can do this by installing homebrew

Install homebrew by running the following command:
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

After installing homebrew, run the following command to install gcc-12:
```bash
brew install gcc@12
```

Make sure that the gcc-12 compiler is in your path.

then add the following to your .bashrc or .zshrc to ensure that nvcc uses the correct compiler.
```bash
export NVCC_PREPEND_FLAGS='-ccbin /home/linuxbrew/.linuxbrew/bin/g++-12'
```

## Step 4: Install OpenVino
install openvino using brew with following command
```bash
/bin/bash brew install openvino
```

## Step 5(optional): Install ccache
> [!WARNING] 
> The whisper repo is very big and takes a long time to compile. To shorten the process of recompiling, it is highly reccomended to use ccache.

To install ccache, run the following command:
### Arch:

```
sudo pacman -S ccache
```

### OpenSUSE:

```bash
sudo zypper install ccache  
```

### Fedora:

```bash
sudo dnf install ccache
```

### Ubuntu:
```bash
sudo apt install ccache
```

# Build and Install the UI

Run the following commands to build and install the UI:

> [!NOTE]
> Please ensure that the application is installed at /usr/bin/koolintelligence. If you have installed it at a different location, please uninstall it, and then reinstall it at the desired location. Otherwise the screenshot functionally will not work.

```bash
cmake -B build/
cmake --build build/
sudo cmake --install build/ #by default installs to /usr/bin, needed for screenshot functionality
```

# Running the UI

Either search for Kool Intelligence or
```bash
koolintelligence
```

# Uninstalling the UI

```bash
sudo cmake --build build/ --target uninstall
```