# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.vividas_drm_utils.Debug:
/Users/yamil.rivera/Repos/wasm_demo/build-darwin/src/libs/drm_utils/Debug/libvividas_drm_utils.a:
	/bin/rm -f /Users/yamil.rivera/Repos/wasm_demo/build-darwin/src/libs/drm_utils/Debug/libvividas_drm_utils.a


PostBuild.vividas_drm_utils.Release:
/Users/yamil.rivera/Repos/wasm_demo/build-darwin/src/libs/drm_utils/Release/libvividas_drm_utils.a:
	/bin/rm -f /Users/yamil.rivera/Repos/wasm_demo/build-darwin/src/libs/drm_utils/Release/libvividas_drm_utils.a


PostBuild.vividas_drm_utils.MinSizeRel:
/Users/yamil.rivera/Repos/wasm_demo/build-darwin/src/libs/drm_utils/MinSizeRel/libvividas_drm_utils.a:
	/bin/rm -f /Users/yamil.rivera/Repos/wasm_demo/build-darwin/src/libs/drm_utils/MinSizeRel/libvividas_drm_utils.a


PostBuild.vividas_drm_utils.RelWithDebInfo:
/Users/yamil.rivera/Repos/wasm_demo/build-darwin/src/libs/drm_utils/RelWithDebInfo/libvividas_drm_utils.a:
	/bin/rm -f /Users/yamil.rivera/Repos/wasm_demo/build-darwin/src/libs/drm_utils/RelWithDebInfo/libvividas_drm_utils.a




# For each target create a dummy ruleso the target does not have to exist
