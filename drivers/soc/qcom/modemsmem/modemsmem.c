/*
 * Copyright (c) 2018 Google Inc.
 *
 *     @file   kernel/drivers/modemsmem/modemsmem.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/ctype.h>
#include <asm/io.h>
#include <soc/qcom/smem.h>
#include "modemsmem.h"
#include <soc/qcom/socinfo.h>

#define BOOTMODE_LENGTH			20
#define FACTORY_STR			"factory"
#define FFBM00_STR			"ffbm-00"
#define FFBM01_STR			"ffbm-01"

static char bootmode[BOOTMODE_LENGTH];

static int __init get_bootmode(char *str)
{
	if (str[0] != '\0')
		strlcpy(bootmode, str, BOOTMODE_LENGTH);

	return 1;
}
__setup("androidboot.mode=", get_bootmode);

static ssize_t modem_smem_show(struct device *d,
			struct device_attribute *attr,
			char *buf)
{
	struct modem_smem_type *modem_smem;

	modem_smem = smem_alloc(SMEM_ID_VENDOR0,
				sizeof(struct modem_smem_type),
				0, SMEM_ANY_HOST_FLAG);
	if (modem_smem) {
		return snprintf(buf, PAGE_SIZE,
				"version:0x%x\n"
				"major_id:0x%x\n"
				"minor_id:0x%x\n"
				"subtype:0x%x\n"
				"platform:0x%x\n"
				"ftm:0x%x\n",
				modem_smem->version,
				modem_smem->major_id,
				modem_smem->minor_id,
				modem_smem->subtype,
				modem_smem->platform,
				modem_smem->ftm_magic);
	} else
		return snprintf(buf, PAGE_SIZE, "The modem smem is null\n");
}

static DEVICE_ATTR(modem_smem, 0664, modem_smem_show, NULL);
static struct attribute *modem_smem_attributes[] = {
	&dev_attr_modem_smem.attr,
	NULL
};

static const struct attribute_group modem_smem_group = {
	.name  = "modemsmem",
	.attrs = modem_smem_attributes,
};

static int modem_smem_probe(struct platform_device *pdev)
{
	int ret = -1;
	struct device_node *np = NULL;
	struct device *dev = NULL;
	struct modem_smem_type *modem_smem;

	pr_debug("[SMEM] %s: Enter probe\n", __func__);

	if (!pdev) {
		pr_err("[SMEM] %s: Invalid pdev\n", __func__);
		return -ENODEV;
	}

	np = pdev->dev.of_node;
	if (!np) {
		pr_err("[SMEM] %s: Invalid DT node\n", __func__);
		return -EINVAL;
	}

	dev = &pdev->dev;
	if (dev == NULL) {
		pr_err("[SMEM] %s: Invalid dev\n", __func__);
		return -EINVAL;
	}

	/* Create sysfs */
	platform_set_drvdata(pdev, dev);
	ret = sysfs_create_group(&pdev->dev.kobj, &modem_smem_group);
	if (ret)
		dev_err(dev, "Failed to create sysfs\n");

	/* Allocate with SMEM channel */
	modem_smem = smem_alloc(SMEM_ID_VENDOR0,
				sizeof(struct modem_smem_type),
				0, SMEM_ANY_HOST_FLAG);
	if (!modem_smem) {
		dev_err(dev, "smem alloc fail\n");
		return -ENOMEM;
	}

	/* Initialize smem with zeros */
	memset_io(modem_smem, 0, sizeof(*modem_smem));

	/* Setup modem SMEM parameters */
	modem_smem_set_u32(modem_smem, version, MODEM_SMEM_VERSION);

	modem_smem_set_u32(modem_smem, major_id,
		(socinfo_get_platform_version() >> 16) & 0xff);

	modem_smem_set_u32(modem_smem, minor_id,
		socinfo_get_platform_version() & 0x00ff);

	modem_smem_set_u32(modem_smem, platform, socinfo_get_platform_type());

	modem_smem_set_u32(modem_smem, subtype, socinfo_get_platform_subtype());

	if ((!strncmp(bootmode, FACTORY_STR, sizeof(FACTORY_STR))) ||
	(!strncmp(bootmode, FFBM00_STR, sizeof(FFBM00_STR))) ||
	(!strncmp(bootmode, FFBM01_STR, sizeof(FFBM01_STR)))) {
		modem_smem_set_u32(modem_smem, ftm_magic, MODEM_FTM_MAGIC);
		dev_info(dev, "Set FTM mode due to %s\n", bootmode);
	}

	dev_dbg(dev, "End probe\n");
	return 0;
}

static const struct of_device_id modem_smem_of_match[] = {
	{ .compatible = "modemsmem" },
	{ }
};
MODULE_DEVICE_TABLE(of, modem_smem_of_match);

static struct platform_driver modem_smem_driver = {
	.probe = modem_smem_probe,
	.driver = {
		.name = "modemsmem",
		.owner = THIS_MODULE,
		.of_match_table = modem_smem_of_match,
	}
};

static int __init modem_smem_init(void)
{
	int ret = -1;

	pr_debug("[SMEM] %s: Enter\n", __func__);

	ret = platform_driver_register(&modem_smem_driver);
	if (ret < 0)
		pr_err("[SMEM] %s: platform_driver register fail. ret:%d\n",
			__func__, ret);

	return ret;
}

static void __exit modem_smem_exit(void)
{
	platform_driver_unregister(&modem_smem_driver);
}

module_init(modem_smem_init);
module_exit(modem_smem_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modem SMEM Driver");
