package com.example.tp1.fragments;

import android.content.DialogInterface;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.app.AlertDialog;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ViewSwitcher;

import com.example.tp1.R;
import com.example.tp1.activities.DrawerMenuActivity;
import com.example.tp1.models.User;
import com.google.gson.Gson;

public class ProfilFragment extends Fragment {
    private User currentUser;

    private ImageButton edit_btn;

    private ViewSwitcher lastname_switcher;
    private ViewSwitcher firstname_switcher;
    private ViewSwitcher email_switcher;
    private ViewSwitcher number_address_switcher;
    private ViewSwitcher street_address_switcher;
    private ViewSwitcher zipcode_address_switcher;
    private ViewSwitcher city_address_switcher;

    private Button save_btn;
    private Button change_pwd_btn;

    private TextView lastname_tv;
    private TextView firstname_tv;
    private TextView mail_tv;
    private TextView number_address_tv;
    private TextView street_address_tv;
    private TextView zipcode_address_tv;
    private TextView city_address_tv;
    private TextView empty_address_tv;

    private EditText lastname_et;
    private EditText firstname_et;
    private EditText mail_et;
    private EditText number_address_et;
    private EditText street_address_et;
    private EditText zipcode_address_et;
    private EditText city_address_et;



    public static ProfilFragment newInstance(){
        return (new ProfilFragment());
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        currentUser = new Gson().fromJson(getArguments().getString("user"), User.class);
        return inflater.inflate(R.layout.fragment_profil, container, false);
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        edit_btn = (ImageButton)getView().findViewById(R.id.fp_icone_edit_profil);

        lastname_switcher = (ViewSwitcher)getView().findViewById(R.id.fp_switcher_lastname);
        firstname_switcher = (ViewSwitcher)getView().findViewById(R.id.fp_switcher_firstname);
        email_switcher = (ViewSwitcher)getView().findViewById(R.id.fp_switcher_mail);
        number_address_switcher = (ViewSwitcher)getView().findViewById(R.id.fp_switcher_number_address);
        street_address_switcher = (ViewSwitcher)getView().findViewById(R.id.fp_switcher_street_address);
        zipcode_address_switcher = (ViewSwitcher)getView().findViewById(R.id.fp_switcher_zipcode_address);
        city_address_switcher = (ViewSwitcher)getView().findViewById(R.id.fp_switcher_city_address);

        save_btn = (Button)getView().findViewById(R.id.fp_save_change_btn);
        change_pwd_btn = (Button)getView().findViewById(R.id.fp_change_pwd_btn);

        lastname_tv = (TextView)getView().findViewById(R.id.fp_placeholder_lastname);
        firstname_tv = (TextView)getView().findViewById(R.id.fp_placeholder_firstname);
        mail_tv = (TextView)getView().findViewById(R.id.fp_placeholder_mail);
        number_address_tv = (TextView)getView().findViewById(R.id.fp_placeholder_number_address);
        street_address_tv = (TextView)getView().findViewById(R.id.fp_placeholder_street_address);
        zipcode_address_tv = (TextView)getView().findViewById(R.id.fp_placeholder_zipcode_address);
        city_address_tv = (TextView)getView().findViewById(R.id.fp_placeholder_city_address);
        empty_address_tv = (TextView)getView().findViewById(R.id.fp_placeholder_empty_address);

        lastname_et = (EditText)getView().findViewById(R.id.fp_placeholder_lastname_edit);
        firstname_et = (EditText)getView().findViewById(R.id.fp_placeholder_firstname_edit);
        mail_et = (EditText)getView().findViewById(R.id.fp_placeholder_mail_edit);
        number_address_et = (EditText)getView().findViewById(R.id.fp_placeholder_number_address_edit);
        street_address_et = (EditText)getView().findViewById(R.id.fp_placeholder_street_address_edit);
        zipcode_address_et = (EditText)getView().findViewById(R.id.fp_placeholder_zipcode_address_edit);
        city_address_et = (EditText)getView().findViewById(R.id.fp_placeholder_city_address_edit);


        change_pwd_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new AlertDialog.Builder(getContext())
                        .setTitle(getResources().getString(R.string.alert_change_pwd_title))
                        .setView(R.layout.alert_dialog_change_pwd)
                        .setMessage(getResources().getString(R.string.alert_chaneg_pwd_message))
                        .setPositiveButton("Change", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                EditText pwd_input = (EditText)getView().findViewById(R.id.alert_dialog_change_pwd_et);
                                EditText pwd_confirm_input = (EditText)getView().findViewById(R.id.alert_dialog_change_pwd_confirmation_et);
                                if(checkPwd(pwd_input.getText().toString(), pwd_confirm_input.getText().toString())){
                                    //requete API pour modifier le mot de passe
                                }
                            }
                        })
                        .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                //nothing
                            }
                        }).setCancelable(false).show();
            }
        });

        save_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                changeLocalUser();
                saveUser();
            }
        });

        edit_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                lastname_switcher.showNext();
                email_switcher.showNext();
                firstname_switcher.showNext();
                number_address_switcher.showNext();
                street_address_switcher.showNext();
                zipcode_address_switcher.showNext();
                city_address_switcher.showNext();
                if(save_btn.getVisibility() == View.VISIBLE){
                    save_btn.setVisibility(View.INVISIBLE);
                } else{
                    save_btn.setVisibility(View.VISIBLE);
                }
            }
        });

        fillProfilFields();
    }

    public void fillProfilFields(){
        lastname_et.setText(currentUser.getLastname());
        lastname_tv.setText(currentUser.getLastname());

        firstname_et.setText(currentUser.getFirstname());
        firstname_tv.setText(currentUser.getFirstname());

        mail_et.setText(currentUser.getEmail());
        mail_tv.setText(currentUser.getEmail());

        if (currentUser.getAddressId() == 0){
            empty_address_tv.setVisibility(View.VISIBLE);
        } else {
            // Get address object with the user object address id
            // Create an address object with the return of the request
            // Fill the fields with the address object
        }

        Toast.makeText(getContext(), currentUser.toString(), Toast.LENGTH_LONG).show();
    }

    public void changeLocalUser(){
        currentUser.setLastname(lastname_et.getText().toString());
        currentUser.setFirstname(firstname_et.getText().toString());

        //checker la validité du mail
        currentUser.setEmail(mail_et.getText().toString());


        //pareil pour l'adresse
    }

    public void saveUser(){
        //request through the FFW API
    }

    public boolean checkPwd(String pwd, String pwdConfirm){
        if (!pwd.equals(pwdConfirm)){
            return false;
        }
        if (pwd.length()<8){
            return false;
        }
        return true;
    }


}
