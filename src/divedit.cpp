
#include "divview.h"
#include "mdview.h"
#include "defview.h"
#include "frame.h"
#include <wx/clipbrd.h>
#include <wx/dialog.h>

enum{
	ID_SmfOutput = 1,
	ID_WosClipboard = 2,
	ID_DefClipboard = 3,
	ID_BmsClipboard = 4,
	ID_DefTransposeUp   = 5,
	ID_DefTransposeDown = 6,
	ID_DefTransposeTo   = 7,
	ID_MidiView = 8
};


BEGIN_EVENT_TABLE(DivisionEditor, wxWindow)
	EVT_SIZE(DivisionEditor::OnSize)
END_EVENT_TABLE()


DivisionEditor::DivisionEditor(FrameWindow *_frame, wxWindow* parent, wxWindowID id,
const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxWindow(parent, id, pos, size, style, name), frame(_frame),
midi(0), defview(0), smf_output(0), wos_clipboard(0), def_clipboard(0), bms_clipboard(0),
division(0)
{
	SetWindowStyle(wxBORDER_NONE | wxCLIP_CHILDREN);
	midi = new MidiView(this, ID_MidiView);
	defview = new DefinitionView(this, 0);
	smf_output = new wxButton(this, ID_SmfOutput, _("Write MIDI File"));
	wos_clipboard = new wxButton(this, ID_WosClipboard, _("Copy woslicer II Cutting Positions"));
	def_clipboard = new wxButton(this, ID_DefClipboard, _("Open BMS WAV definitions..."));
	bms_clipboard = new wxButton(this, ID_BmsClipboard, _("Copy BMS Data to Clipboard"));
	def_transpose_up   = new wxButton(this, ID_DefTransposeUp, _("<"));
	def_transpose_down = new wxButton(this, ID_DefTransposeDown, _(">"));
	def_transpose_to   = new wxButton(this, ID_DefTransposeTo, _("Transpose to..."));
	Connect(ID_SmfOutput, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DivisionEditor::OnSmfOut));
	Connect(ID_WosClipboard, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DivisionEditor::OnDivCopy));
	Connect(ID_DefClipboard, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DivisionEditor::OnDefOut));
	Connect(ID_BmsClipboard, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DivisionEditor::OnSeqCopy));
	Connect(ID_MidiView, wxEVT_SET_STATUS_TEXT, wxCommandEventHandler(DivisionEditor::OnSetStatusText));
	Connect(ID_DefTransposeUp, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DivisionEditor::OnDefTransposeUp));
	Connect(ID_DefTransposeDown, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DivisionEditor::OnDefTransposeDown));
	Connect(ID_DefTransposeTo, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DivisionEditor::OnDefTransposeTo));
	midi->SetMidiData(0);
	midi->Show();
}


void DivisionEditor::OnSize(wxSizeEvent &WXUNUSED(event)){
	_Size();
}

void DivisionEditor::OnSetStatusText(wxCommandEvent &event){
	frame->UpdateStatus(event.GetString());
}

void DivisionEditor::SetDivision(Division *_div){
	if (division = _div){
		midi->SetMidiData(division);
		defview->SetDivision(division);
		smf_output->Enable(true);
		wos_clipboard->Enable(true);
		def_clipboard->Enable(true);
		bms_clipboard->Enable(true);
		def_transpose_up->Enable(true);
		def_transpose_down->Enable(true);
		def_transpose_to->Enable(true);
	}else{
		midi->SetMidiData(0);
		defview->SetDivision(0);
		smf_output->Enable(false);
		wos_clipboard->Enable(false);
		def_clipboard->Enable(false);
		bms_clipboard->Enable(false);
		def_transpose_up->Enable(false);
		def_transpose_down->Enable(false);
		def_transpose_to->Enable(false);
	}
}


void DivisionEditor::_Size(){
	static const int wr = 200, hd = 60, hi = 0, tw = 80;
	wxSize size = GetClientSize();
	midi->SetSize(0, 0, size.x-wr, size.y-hd-hi);
	defview->SetSize(0, size.y-hd-hi, size.x-wr-tw, hd);

	smf_output->SetSize(size.x-wr, 0, wr, (size.y-hd-hi)/2);
	wos_clipboard->SetSize(size.x-wr, (size.y-hd-hi)/2, wr, size.y-hd-hi-(size.y-hd-hi)/2);
	def_clipboard->SetSize(size.x-wr, size.y-hd-hi, wr, hd/2);
	bms_clipboard->SetSize(size.x-wr, size.y-hd-hi+hd/2, wr, hd-hd/2);
	def_transpose_up->SetSize(size.x-wr-tw, size.y-hd-hi, tw/2, hd/2);
	def_transpose_down->SetSize(size.x-wr-tw+tw/2, size.y-hd-hi, tw-tw/2, hd/2);
	def_transpose_to->SetSize(size.x-wr-tw, size.y-hd-hi+hd/2, tw, hd-hd/2);
}

void DivisionEditor::_SmfOut(){
	if (!division) return;
	auto filename = wxFileSelector(
	        _("MIDI files"),
	        wxString(),
	        wxString(),
	        _("mid"),
	        midi_file_filter,
	        wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
	        this
    );

	if (filename.empty()) return;

	if (!division->save_smf(filename)){
		wxMessageBox( _("Failed to export MIDI file."), app_name, wxOK | wxICON_EXCLAMATION, this );
	}
}

void DivisionEditor::_DivCopy(){
	if (!division) return;
	wxString data = division->get_div_sequence_data();
	if (wxTheClipboard->Open()){
		wxTheClipboard->SetData( new wxTextDataObject(data) );
		wxTheClipboard->Close();
	}else{
		wxMessageBox( _("Clipboard is not available."), app_name, wxOK | wxICON_EXCLAMATION, this );
	}
}

void DivisionEditor::_SeqCopy(){
	if (!division) return;
	wxString data = division->get_bms_sequence_data(frame->project->GetSource());
	if (wxTheClipboard->Open()){
		wxTheClipboard->SetData( new wxTextDataObject(data) );
		wxTheClipboard->Close();
	}else{
		wxMessageBox( _("Clipboard is not available."), app_name, wxOK | wxICON_EXCLAMATION, this );
	}
}

void DivisionEditor::OnDefTransposeUp(wxCommandEvent &WXUNUSED(event)){
	division->def_transpose_up();
	defview->ReloadDivision();
	defview->Update();
}

void DivisionEditor::OnDefTransposeDown(wxCommandEvent &WXUNUSED(event)){
	division->def_transpose_down();
	defview->ReloadDivision();
	defview->Update();
}


class DefTransposeDialog : public wxDialog{

	enum{
		ID_Edit = 1
	};

	wxTextCtrl *edit;
	wxButton *ok;
	ZZNumber data;
	bool zz_enabled;

public:
	DefTransposeDialog(wxWindow *owner, ZZNumber nbegin, bool zz_enabled) :
		wxDialog(owner, -1, _("WAV definitions"), wxDefaultPosition, wxSize(200,120), wxDEFAULT_DIALOG_STYLE),
		edit(new wxTextCtrl(this, ID_Edit)),
		ok(new wxButton(this, wxID_OK, _("OK"))),
		data(nbegin), zz_enabled(zz_enabled)
	{
		int w, h;
		this->GetClientSize(&w, &h);
		edit->SetSize(5, 5, w-10, h-40);
		ok->SetSize(5, h-30, w-10, 25);
		SetEscapeId(wxID_CANCEL);
		edit->SetValue(nbegin.to_string());
	}
	~DefTransposeDialog(){}
	ZZNumber get_value(){
		return data;
	}
	void OnOK(wxCommandEvent &WXUNUSED(ev)){
		wxString s = edit->GetValue();
		if (s.length() != 2 || !data.from_string(s) || (!zz_enabled && !data.in_ff())){
			MessageBeep(0); // windows
			return;
		}
		this->EndDialog(wxID_OK);
	}
	DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(DefTransposeDialog, wxDialog)
	EVT_COMMAND(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, DefTransposeDialog::OnOK)
END_EVENT_TABLE()


void DivisionEditor::OnDefTransposeTo(wxCommandEvent &WXUNUSED(event)){
	if (division->definitions_count()==0) return;
	DefTransposeDialog *dialog = new DefTransposeDialog(frame, division->definition(0).zz, division->is_zz_enabled());
	int ret = dialog->ShowModal();
	if (ret != wxID_OK){
		return;
	}
	ZZNumber zz = dialog->get_value();
	division->def_transpose_to(zz);
	defview->ReloadDivision();
	defview->Update();
}



//--------------------------------------------------------------------------------------



class DefDialog : public wxDialog{

	enum{
		ID_Label = 1,
		ID_Name,
		ID_Data,
		ID_COPY
	};

	Division *div;
	wxStaticText *label;
	wxTextCtrl *name;
	wxTextCtrl *data;
	wxButton *copy;
	wxButton *close;

	void _Size(){
		wxSize size = GetClientSize();
		if (label) label->SetSize(5, 8, 80, 15);
		if (name) name->SetSize(85, 5, size.x-90, 18);
		if (data) data->SetSize(5, 30, size.x-10, size.y-65);
		if (copy) copy->SetSize(5, size.y-30, (size.x-10)/2, 25);
		if (close) close->SetSize((size.x-10)/2+5, size.y-30, (size.x-10)/2, 25);
	}
	void _NameUpdated(){
		wxString n = name->GetValue();
		wxString d;
		int ofs = div->head_margin_exists() ? 1 : 0;
		for (size_t i=0; i<div->definitions_count(); i++){
			const Definition &def = div->definition(i);
			d += wxString::Format(_("#WAV%s %s_%03d.wav\r\n"), def.zz.to_string(), n, def.div+ofs);
		}
		data->SetValue(d);
	}
	void OnSize(wxSizeEvent &e){ _Size(); }
	void OnNameUpdate(wxCommandEvent &e){ _NameUpdated(); }
public:
	DefDialog(wxWindow *owner, Division *_div, const wxString &def_name=_("untitled")) :
		wxDialog(owner, -1, _("BMS WAV definitions"), wxDefaultPosition, wxSize(400,300), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
		label(new wxStaticText(this, ID_Label, _("Filename (&F):"))),
		name(new wxTextCtrl(this, ID_Name)),
		data(new wxTextCtrl(this, ID_Data, _(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_TAB)),
		copy(new wxButton(this, ID_COPY, _("Copy to clipboard (&C)"))),
		close(new wxButton(this, wxID_CANCEL, _("Close (&W)"))),
		div(_div)
	{
		SetEscapeId(wxID_CANCEL);
		_Size();
		name->SetValue(def_name.empty()? _("untitled") : def_name);
		copy->SetDefault();
	}
	~DefDialog(){}
	void OnCopy(wxCommandEvent &WXUNUSED(event)){
		wxString s = data->GetValue();
		if (wxTheClipboard->Open()){
			wxTheClipboard->SetData( new wxTextDataObject(s) );
			wxTheClipboard->Close();
		}else{
			wxMessageBox( _("Clipboard is not available."), app_name, wxOK | wxICON_EXCLAMATION, this );
		}
	}

	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(DefDialog, wxDialog)
	EVT_COMMAND(DefDialog::ID_COPY, wxEVT_COMMAND_BUTTON_CLICKED, DefDialog::OnCopy)
	EVT_SIZE(DefDialog::OnSize)
	EVT_TEXT(DefDialog::ID_Name, OnNameUpdate)
END_EVENT_TABLE()

void DivisionEditor::_DefOut(){
	if (!division) return;
	wxString def_name = frame->project->GetTitle();
	DefDialog dialog (frame, division, def_name);
	dialog.ShowModal();
	dialog.Destroy();
}


