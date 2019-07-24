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
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xb5a459dc, __VMLINUX_SYMBOL_STR(unregister_blkdev) },
	{ 0x46b93b95, __VMLINUX_SYMBOL_STR(blk_cleanup_queue) },
	{ 0x155ee2dc, __VMLINUX_SYMBOL_STR(put_disk) },
	{ 0x48806219, __VMLINUX_SYMBOL_STR(del_gendisk) },
	{ 0xebeb685, __VMLINUX_SYMBOL_STR(add_disk) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x63749484, __VMLINUX_SYMBOL_STR(blk_init_queue) },
	{ 0x12552412, __VMLINUX_SYMBOL_STR(alloc_disk) },
	{ 0x71a50dbc, __VMLINUX_SYMBOL_STR(register_blkdev) },
	{ 0x81fcd7c8, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x92a94ad2, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x3356b90b, __VMLINUX_SYMBOL_STR(cpu_tss) },
	{ 0x5ed5518e, __VMLINUX_SYMBOL_STR(__blk_end_request_cur) },
	{ 0x59940c13, __VMLINUX_SYMBOL_STR(blk_fetch_request) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "CE73F9963B06A1FB63F7615");
