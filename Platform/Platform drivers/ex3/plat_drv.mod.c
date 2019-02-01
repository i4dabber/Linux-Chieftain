#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x83f9a31d, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x9257e484, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x9b62a035, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xca37bda2, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xb214bf7c, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x3ff2e168, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xf13781a7, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xc31596a7, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xb52aa66f, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x2b9d393a, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xf2a0788, __VMLINUX_SYMBOL_STR(gpiod_direction_output_raw) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
	{ 0xcbffac13, __VMLINUX_SYMBOL_STR(of_property_read_variable_u32_array) },
	{ 0x9509ae4c, __VMLINUX_SYMBOL_STR(of_get_named_gpio_flags) },
	{ 0xc8c38926, __VMLINUX_SYMBOL_STR(of_count_phandle_with_args) },
	{ 0xc783e105, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0xa0d5ba68, __VMLINUX_SYMBOL_STR(gpiod_set_raw_value) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x28cc25db, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0x1e047854, __VMLINUX_SYMBOL_STR(warn_slowpath_fmt) },
	{ 0xf4fa543b, __VMLINUX_SYMBOL_STR(arm_copy_to_user) },
	{ 0x97255bdf, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x95de4ef5, __VMLINUX_SYMBOL_STR(gpiod_get_raw_value) },
	{ 0xc4cd8ca0, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "0B49DAB8D067493C818CDB1");
