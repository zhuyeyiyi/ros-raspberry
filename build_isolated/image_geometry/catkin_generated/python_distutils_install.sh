#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/pi/catkinws/src/vision_opencv/image_geometry"

# snsure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/pi/catkinws/install_isolated/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/pi/catkinws/install_isolated/lib/python2.7/dist-packages:/home/pi/catkinws/build_isolated/image_geometry/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/pi/catkinws/build_isolated/image_geometry" \
    "/usr/bin/python" \
    "/home/pi/catkinws/src/vision_opencv/image_geometry/setup.py" \
    build --build-base "/home/pi/catkinws/build_isolated/image_geometry" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/home/pi/catkinws/install_isolated" --install-scripts="/home/pi/catkinws/install_isolated/bin"
