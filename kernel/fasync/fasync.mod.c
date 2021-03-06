#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xc6c01fa, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x840c7614, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x67e6ac0a, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xbf7ea475, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x31ed28cb, __VMLINUX_SYMBOL_STR(device_remove_file) },
	{ 0x2cfebb54, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0xe2d432f9, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x5b1b1532, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x5e1b802f, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x914eafcc, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x1fa72bac, __VMLINUX_SYMBOL_STR(cdev_alloc) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x6ca0693f, __VMLINUX_SYMBOL_STR(fasync_helper) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xee584286, __VMLINUX_SYMBOL_STR(kill_fasync) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "C845173C1A046B7937C1ACF");
