#ifndef _BACKPORT_LINUX_PCI_H
#define _BACKPORT_LINUX_PCI_H
#include_next <linux/pci.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(5,4,0)
#include <linux/pci-aspm.h>
#endif

#if defined(CONFIG_PCI)
#if LINUX_VERSION_IS_LESS(5,3,0)
static inline int
backport_pci_disable_link_state(struct pci_dev *pdev, int state)
{
	u16 aspmc;

	pci_disable_link_state(pdev, state);

	pcie_capability_read_word(pdev, PCI_EXP_LNKCTL, &aspmc);
	if ((state & PCIE_LINK_STATE_L0S) &&
	    (aspmc & PCI_EXP_LNKCTL_ASPM_L0S))
		return -EPERM;

	if ((state & PCIE_LINK_STATE_L1) &&
	    (aspmc & PCI_EXP_LNKCTL_ASPM_L1))
		return -EPERM;

	return 0;
}
#define pci_disable_link_state LINUX_BACKPORT(pci_disable_link_state)

#endif /* < 5.3 */
#endif /* defined(CONFIG_PCI) */

#ifndef PCI_IRQ_INTX
#define PCI_IRQ_INTX PCI_IRQ_LEGACY
#endif

#if LINUX_VERSION_IS_LESS(6,13,0)

#define pcim_request_all_regions(pdev, name) pcim_iomap_regions_request_all(pdev, BIT(0), name)

static inline void __iomem *
backport_pcim_iomap(struct pci_dev *pdev, int bar, unsigned long maxlen)
{
	void __iomem * const *table = pcim_iomap_table(pdev);

	if (!table)
		return NULL;

	return table[0];
}
#define pcim_iomap LINUX_BACKPORT(pcim_iomap)

#endif /* <6.13 */

#endif /* _BACKPORT_LINUX_PCI_H */
