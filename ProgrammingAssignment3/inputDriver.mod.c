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
	{ 0x884142c7, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0x9143ad63, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x1f2462ec, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0x6815413c, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xc0391004, __VMLINUX_SYMBOL_STR(mutex_trylock) },
	{ 0x1fa20e73, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbe8bc285, __VMLINUX_SYMBOL_STR(class_unregister) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
	{ 0x26abc5be, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xe9278edd, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x4436b614, __VMLINUX_SYMBOL_STR(__class_create) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "17632C8D9221CA9783596AA");
