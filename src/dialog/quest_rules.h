#ifndef ZC_DIALOG_QRS_H
#define ZC_DIALOG_QRS_H

#include <gui/dialog.h>
#include <gui/text_field.h>
#include <gui/list_data.h>
#include <gui/window.h>
#include <functional>
#include <string_view>

#define QR_SZ QUESTRULES_NEW_SIZE

void call_qr_dialog(size_t qrs_per_tab, std::function<void(byte*)> setQRs);
void call_qrsearch_dialog(std::function<void(byte*)> setQRs);

class QRDialog: public GUI::Dialog<QRDialog>
{
public:
	enum class message { OK, CANCEL, TOGGLE_QR, HEADER, RULESET,
		CHEATS, RULETMP, QRSTR_CPY, QRSTR_LOAD, RERUN, SEARCH };

	QRDialog(byte const* qrs, size_t qrs_per_tab, std::function<void(byte*)> setQRs);

	std::shared_ptr<GUI::Widget> view() override;
	bool handleMessage(const GUI::DialogMessage<message>& msg);
	void reloadQRs();
private:
	std::shared_ptr<GUI::TextField> mapCountTF;
	std::shared_ptr<GUI::Window> window;
	
	std::function<void(byte*)> setQRs;
	byte local_qrs[QR_SZ];
	byte const* realqrs;
	size_t qrs_per_tab;
	bool searchmode;
	
	friend void call_qrsearch_dialog(std::function<void(byte*)> setQRs);
};

#endif

