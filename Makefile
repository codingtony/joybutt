include $(TOPDIR)/rules.mk 
PKG_NAME:=joybutt
PKG_RELEASE:=1
PKG_VERSION:=0.2
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)
include $(INCLUDE_DIR)/package.mk

define Package/joybutt
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Utility for listening to joystick button events
endef

define Package/joybutt/description 
	Prints joystick button event on stdout
endef 

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/ 
endef

define Package/joybutt/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/joybutt $(1)/usr/bin/
endef

$(eval $(call BuildPackage,joybutt)) 
