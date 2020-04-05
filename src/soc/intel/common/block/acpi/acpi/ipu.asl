/* SPDX-License-Identifier: GPL-2.0-only */
/* This file is part of the coreboot project. */

/* IPU3 input system - Device 05, Function 0 */
Device (IMGU)
{
	Name (_ADR, 0x00050000)
	Name (_DDN, "Imaging Unit")
	Name (_CCA, ZERO)
	Name (CAMD, 0x01)
}
