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
	{ 0x63352dba, __VMLINUX_SYMBOL_STR(kobject_del) },
	{ 0x390b5752, __VMLINUX_SYMBOL_STR(kset_unregister) },
	{ 0xa7233ac8, __VMLINUX_SYMBOL_STR(sysfs_remove_file_ns) },
	{ 0xf16343f3, __VMLINUX_SYMBOL_STR(sysfs_create_file_ns) },
	{ 0x4255026d, __VMLINUX_SYMBOL_STR(kobject_init_and_add) },
	{ 0x96626387, __VMLINUX_SYMBOL_STR(kset_create_and_add) },
	{ 0x81fcd7c8, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x92a94ad2, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x9273028d, __VMLINUX_SYMBOL_STR(kobject_create_and_add) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x1f8b534c, __VMLINUX_SYMBOL_STR(kobject_uevent) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "89F44BE1D0209B6FA3BECE9");
