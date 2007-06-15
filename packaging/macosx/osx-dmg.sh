#!/bin/sh
#
# Inkscape packaging script for Mac OS X
#
# The script creates a read-write disk image, 
# copies Inkscape in it, copies additional python packages
# if needed, customizes its appearance using a 
# previously created .DS_Store file (inkscape*.ds_store),
# and then compresses the disk image for distribution.
#
# Authors:
#	Jean-Olivier Irisson <jo.irisson@gmail.com>
#	Michael Wybrow <mjwybrow@users.sourceforge.net>
#
# Copyright 2006
# Licensed under GNU General Public License
#
#
# How to update the disk image layout:
# ------------------------------------
#
# Modify the 'dmg_background.svg' file and generate a new 
# 'dmg_background.png' file.
#
# Update the AppleScript file 'dmg_set_style.scpt'.
#
# Run this script with the '-s' option.  It will apply the
# 'dmg_set_style.scpt' AppleScript file, and then prompt the
# user to check the window size and position before writing
# a new 'inkscape.ds_store' file to work around a bug in Finder
# and AppleScript.  The updated 'inkscape.ds_store' will need 
# to be commited to the repository when this is done.
#

# Defaults
set_ds_store=false
add_python=false
ds_store_file="inkscape.ds_store"
package="Inkscape.app"
RWNAME="RWinkscape.dmg"
VOLNAME="Inkscape"
TMPDIR="/tmp/dmg-$$"
AUTOOPENOPT=

# Parse command line arguments
while [ "$1" != "" ]
do
	case $1 in
	  	-s)
			set_ds_store=true ;;
		-py|--with-python)
			add_python=true
			python_dir="$2"
			ds_store_file="inkscape_python.ds_store"
			shift 1 ;;
	esac
	shift 1
done
# some checks
if [ ! -e "$package" ]; then
	echo "Cannot find $package"
	exit 1
fi
if [ ${add_python} = "true" ]; then
	if [ ! -e "$python_dir" ]; then
		echo "Cannot find your python packages directory"
		exit 1
	fi
fi

# Create temp directory with desired contents of the release volume.
rm -rf "$TMPDIR"
mkdir "$TMPDIR"

echo "Copying files to temp directory..."
# Inkscape itself
# copy Inkscape.app
cp -rf "$package" "$TMPDIR"/
# link to Applications in order to drag and drop inkscape onto it.
ln -sf /Applications "$TMPDIR"/

# Python
if [ ${add_python} = "true" ]; then
	# copy python libraries
	cp -rf "$python_dir"/* "$TMPDIR"/
	# link python environment in order to drag and drop inkscape onto it
	ln -sf /System/Library/Frameworks/Python.framework/Versions/2.3/lib/python2.3/site-packages "$TMPDIR"/Python\ site-packages
fi

# Copy a background image inside a hidden directory so the image
# file itself won't be shown.
mkdir "$TMPDIR/.background"
cp dmg_background.png "$TMPDIR/.background/background.png"

# If the appearance settings are not to be modified we just copy them
if [ ${set_ds_store} = "false" ]; then
	# Copy the .DS_Store file which contains information about
	# window size, appearance, etc.  Most of this can be set
	# with Apple script but involves user intervention so we
	# just keep a copy of the correct settings and use that instead.
	cp $ds_store_file "$TMPDIR/.DS_Store"
	AUTOOPENOPT=-noautoopen
fi

# Create a new RW image from the temp directory.
echo "Creating a new RW disk image..."
rm -f "$RWNAME"
/usr/bin/hdiutil create -srcfolder "$TMPDIR" -volname "$VOLNAME" -fs HFS+ -fsargs "-c c=64,a=16,e=16" -format UDRW "$RWNAME"

# We're finished with the temp directory, remove it.
rm -rf "$TMPDIR"

# Mount the created image.
echo "Mounting the disk image..."
MOUNT_DIR="/Volumes/$VOLNAME"
DEV_NAME=`/usr/bin/hdiutil attach -readwrite -noverify $AUTOOPENOPT  "$RWNAME" | egrep '^/dev/' | sed 1q | awk '{print $1}'`

# Have the disk image window open automatically when mounted.
bless -openfolder /Volumes/$VOLNAME

# In case the apperance has to be modified, mount the image and apply the base settings to it via Applescript
if [ ${set_ds_store} = "true" ]; then
	/usr/bin/osascript dmg_set_style.scpt

	open "/Volumes/$VOLNAME"
	# BUG: one needs to move and close the window manually for the
	# changes in appearance to be retained... 
        echo " 
        ************************************** 
        *  Please move the disk image window * 
        *    to the center of the screen     *  
        *   then close it and press enter    * 
        ************************************** 
        " 
        read -e DUMB

	# .DS_Store files aren't written till the disk is unmounted, 
	# or finder is restarted.
	hdiutil detach "$DEV_NAME"
	AUTOOPENOPT=-noautoopen
	DEV_NAME=`/usr/bin/hdiutil attach -readwrite -noverify $AUTOOPENOPT  "$RWNAME" | egrep '^/dev/' | sed 1q | awk '{print $1}'`
	echo
	echo "New $ds_store_file file written. Re-run $0 without the -s option to use it"
	cp /Volumes/$VOLNAME/.DS_Store ./$ds_store_file
	SetFile -a v ./$ds_store_file

	# Unmount the disk image.
	hdiutil detach "$DEV_NAME"
	rm -f "$RWNAME"

	exit 0
fi

# Unmount the disk image.
hdiutil detach "$DEV_NAME"

# Create the offical release image by compressing the RW one.
/usr/bin/hdiutil convert "$RWNAME" -format UDZO -imagekey zlib-level=9 -o "Inkscape.dmg"
rm -f "$RWNAME"

exit 0
