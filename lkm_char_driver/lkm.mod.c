#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x0bc5fb0d, "unregister_chrdev_region" },
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0x7851be11, "__SCT__might_resched" },
	{ 0x092a35a2, "_copy_to_user" },
	{ 0x7a5ffe84, "init_wait_entry" },
	{ 0xd272d446, "schedule" },
	{ 0x0db8d68d, "prepare_to_wait_event" },
	{ 0xc87f4bab, "finish_wait" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xc0f19660, "remove_proc_entry" },
	{ 0x40a621c5, "snprintf" },
	{ 0x546c19d9, "validate_usercopy_range" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0xd272d446, "__fentry__" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xe8213e80, "_printk" },
	{ 0x9f222e1e, "alloc_chrdev_region" },
	{ 0x653aa194, "class_create" },
	{ 0xe486c4b7, "device_create" },
	{ 0xd5f66efd, "cdev_init" },
	{ 0x8ea73856, "cdev_add" },
	{ 0x80222ceb, "proc_create" },
	{ 0xbebe66ff, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x0bc5fb0d,
	0xbd03ed67,
	0x7851be11,
	0x092a35a2,
	0x7a5ffe84,
	0xd272d446,
	0x0db8d68d,
	0xc87f4bab,
	0xd272d446,
	0xc0f19660,
	0x40a621c5,
	0x546c19d9,
	0xa61fd7aa,
	0xd272d446,
	0xd272d446,
	0xe8213e80,
	0x9f222e1e,
	0x653aa194,
	0xe486c4b7,
	0xd5f66efd,
	0x8ea73856,
	0x80222ceb,
	0xbebe66ff,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"unregister_chrdev_region\0"
	"__ref_stack_chk_guard\0"
	"__SCT__might_resched\0"
	"_copy_to_user\0"
	"init_wait_entry\0"
	"schedule\0"
	"prepare_to_wait_event\0"
	"finish_wait\0"
	"__stack_chk_fail\0"
	"remove_proc_entry\0"
	"snprintf\0"
	"validate_usercopy_range\0"
	"__check_object_size\0"
	"__fentry__\0"
	"__x86_return_thunk\0"
	"_printk\0"
	"alloc_chrdev_region\0"
	"class_create\0"
	"device_create\0"
	"cdev_init\0"
	"cdev_add\0"
	"proc_create\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "94E5F2A918064E29E05FC75");
