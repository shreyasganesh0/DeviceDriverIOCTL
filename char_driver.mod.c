#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x10\x00\x00\x00\xd8\x7e\x99\x92"
	"_printk\0"
	"\x1c\x00\x00\x00\x73\xe5\xd0\x6b"
	"down_interruptible\0\0"
	"\x0c\x00\x00\x00\x66\x69\x2a\xcf"
	"up\0\0"
	"\x18\x00\x00\x00\xe8\x88\x71\x84"
	"krealloc_noprof\0"
	"\x1c\x00\x00\x00\x91\xc9\xc5\x52"
	"__kmalloc_noprof\0\0\0\0"
	"\x18\x00\x00\x00\xed\x52\x0c\x1f"
	"class_create\0\0\0\0"
	"\x1c\x00\x00\x00\xc0\xfb\xc3\x6b"
	"__unregister_chrdev\0"
	"\x20\x00\x00\x00\x3b\x8f\xd7\x3f"
	"register_chrdev_region\0\0"
	"\x18\x00\x00\x00\x26\x3f\xea\xb5"
	"device_create\0\0\0"
	"\x14\x00\x00\x00\xf7\x9e\x07\xbe"
	"cdev_init\0\0\0"
	"\x14\x00\x00\x00\x08\xa6\xed\xa0"
	"cdev_add\0\0\0\0"
	"\x14\x00\x00\x00\x26\xc0\x83\x3d"
	"cdev_del\0\0\0\0"
	"\x10\x00\x00\x00\xba\x0c\x7a\x03"
	"kfree\0\0\0"
	"\x18\x00\x00\x00\xf1\x53\xb8\x92"
	"device_destroy\0\0"
	"\x18\x00\x00\x00\x54\xac\xd9\xd7"
	"class_destroy\0\0\0"
	"\x1c\x00\x00\x00\x48\x9f\xdb\x88"
	"__check_object_size\0"
	"\x20\x00\x00\x00\x28\xe1\xa4\x12"
	"__arch_copy_from_user\0\0\0"
	"\x1c\x00\x00\x00\xef\x6d\x5c\xa6"
	"alt_cb_patch_nops\0\0\0"
	"\x10\x00\x00\x00\xad\x64\xb7\xdc"
	"memset\0\0"
	"\x1c\x00\x00\x00\x54\xfc\xbb\x6c"
	"__arch_copy_to_user\0"
	"\x24\x00\x00\x00\x33\xb3\x91\x60"
	"unregister_chrdev_region\0\0\0\0"
	"\x18\x00\x00\x00\xc1\x79\xf3\xc3"
	"param_ops_int\0\0\0"
	"\x18\x00\x00\x00\x99\x94\xe5\x70"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "1AEC7893AD6C366E8F145EF");
