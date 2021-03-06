
#pragma once

#include "common.h"
#include "gui.h"
#include "project.h"

class FrameWindow;
class MidiView;
class DefinitionView;


class DivisionEditor : public wxWindow{

	FrameWindow *frame;
	MidiView *midi;
	DefinitionView *defview;
	wxButton *smf_output, *wos_clipboard;
	wxButton *def_clipboard, *bms_clipboard;
	wxButton *def_transpose_up, *def_transpose_down, *def_transpose_to;
	Division *division;

	void _Size();
	void _SmfOut();
	void _DivCopy();
	void _DefOut();
	void _SeqCopy();

public:
	DivisionEditor(FrameWindow *_frame, wxWindow* parent, wxWindowID id,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = wxCLIP_CHILDREN,
		const wxString& name = _("DivisionsEditor"));

	void SetDivision(Division *_div);
	DECLARE_EVENT_TABLE()
public:
	void OnSize(wxSizeEvent &event);
	void OnSmfOut(wxCommandEvent &event){ _SmfOut(); }
	void OnDivCopy(wxCommandEvent &event){ _DivCopy(); }
	void OnDefOut(wxCommandEvent &event){ _DefOut(); }
	void OnSeqCopy(wxCommandEvent &event){ _SeqCopy(); }
	void OnSmfOut(wxMenuEvent &event){ _SmfOut(); }
	void OnDivCopy(wxMenuEvent &event){ _DivCopy(); }
	void OnDefOut(wxMenuEvent &event){ _DefOut(); }
	void OnSeqCopy(wxMenuEvent &event){ _SeqCopy(); }
	void OnSetStatusText(wxCommandEvent &event);
	void OnDefTransposeUp(wxCommandEvent &event);
	void OnDefTransposeDown(wxCommandEvent &event);
	void OnDefTransposeTo(wxCommandEvent &event);
};



class DivisionsView : public wxWindow{

	friend class FrameWindow;
	FrameWindow *frame;

	wxButton *new_div;
	wxListBox *divisions;
	DivisionEditor *editor;

	void _Size();
	void _NewDivision();

public:
	DivisionsView(FrameWindow *_frame, wxWindow* parent, wxWindowID id,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = wxCLIP_CHILDREN,
		const wxString& name = _("DivisionsView"));
	~DivisionsView();

	void OnSize(wxSizeEvent &event);
	void OnNewDivision(wxCommandEvent &event);
	void OnNewDivision(wxMenuEvent &event);
	void OnDeleteDivision(wxMenuEvent &event);
	void OnSelectDivision(wxCommandEvent &event);
	void ProjectChanged();
	void DivisionChanged();

	DECLARE_EVENT_TABLE()
};


