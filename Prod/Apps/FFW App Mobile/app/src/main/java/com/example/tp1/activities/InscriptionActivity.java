package com.example.tp1.activities;

import android.app.DownloadManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.android.volley.AuthFailureError;
import com.android.volley.DefaultRetryPolicy;
import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.HttpHeaderParser;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.example.tp1.R;
import com.example.tp1.models.Address;
import com.example.tp1.models.User;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;
import java.util.regex.Pattern;

public class InscriptionActivity extends AppCompatActivity {
    private EditText inputFirstname;
    private EditText inputLastname;
    private EditText inputMdp;
    private EditText inputMdpConfirm;
    private EditText inputMail;
    private EditText inputAdress;
    private EditText inputComplement;
    private EditText inputZipCode;
    private EditText inputCity;
    private EditText inputCountry;
    private Button signinBtn;
    private TextView signinError;

    private String firstname;
    private String lastname;
    private String mdp;
    private String mdpConfirm;
    private String mail;
    private String adress;
    private String complement;
    private String zipCode;
    private String city;
    private String country;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_inscription);

        //toolbar configuration
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setTitle("Inscription");

        //initialization
        inputFirstname = (EditText)findViewById(R.id.sign_in_firstname);
        inputLastname = (EditText)findViewById(R.id.sign_in_lastname);
        inputMdp = (EditText)findViewById(R.id.sign_in_user_mdp);
        inputMdpConfirm = (EditText)findViewById(R.id.sign_in_user_mdp_confirm);
        inputMail = (EditText)findViewById(R.id.sign_in_user_mail);
        inputAdress = (EditText)findViewById(R.id.sign_in_user_adress);
        inputComplement = (EditText)findViewById(R.id.sign_in_user_adress_complement);
        inputZipCode = (EditText)findViewById(R.id.sign_in_zipcode);
        inputCity = (EditText)findViewById(R.id.sign_in_city);
        inputCountry = (EditText)findViewById(R.id.sign_in_country);
        signinBtn = (Button)findViewById(R.id.sign_in_validation_btn);
        signinError = (TextView)findViewById(R.id.sign_in_error_message);

        //configure signinBtn
        signinBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                firstname = inputFirstname.getText().toString();
                lastname = inputLastname.getText().toString();
                mdp = inputMdp.getText().toString();
                mdpConfirm = inputMdpConfirm.getText().toString();
                mail = inputMail.getText().toString();
                adress = inputAdress.getText().toString();
                complement = inputComplement.getText().toString();
                zipCode = inputZipCode.getText().toString();
                city = inputCity.getText().toString();
                country = inputCountry.getText().toString();
                if (isFormOk(firstname, lastname, mdp, mdpConfirm, mail)) {
                    createUser(firstname, lastname, mdp, mail, signinError);
                    //if(isAdressOk(adress, zipCode,city, country)){
                    //    Address userAddress = createAdress(adress, zipCode, complement, city, country);
                    //    addAdressToUser
                    //}

                } else {
                    signinError.setText("Tous les champs '*' doivent être remplis et le mot de passe doit être de 8 caractères minimum.");
                }
            }
        });

    }

    public boolean isFormOk(String fn, String ln, String mdp, String mdpConfirm, String mail){
        boolean isOk = true;
        if(fn.length()<1 || ln.length()<1 || mdp.length()<1 || mdpConfirm.length()<1 || mail.length()<1){
            isOk = false;
        }
        if(!mdp.equals(mdpConfirm)){
            isOk = false;
        }
        if(Pattern.matches("^[_a-z0-9-]+(\\.[_a-z0-9-]+)*@[a-z0-9-]+(\\.[a-z0-9-]+)+$",mail) == false){
            isOk = false;
        }
        return isOk;
    }

    public void createUser(String firstname, String lastname, String mdp, String mail, final TextView signinError){
        try {
            RequestQueue requestQueue = Volley.newRequestQueue(this);
            String URL = getResources().getString(R.string.localhost) + "/users";

            //prepare le body de la requete :
            JSONObject jsonBody = new JSONObject();
            jsonBody.put("firstname",firstname);
            jsonBody.put("lastname", lastname);
            jsonBody.put("mdp",mdp);
            jsonBody.put("email",mail);
            final String requestBody = jsonBody.toString();

            StringRequest stringRequest = new StringRequest(
                    Request.Method.POST,
                    URL,
                    new Response.Listener<String>() {
                        @Override
                        public void onResponse(String response) {
                            signinError.setText(response);
                        }
                    }, new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    signinError.setText(error.toString());
                }
            }) {
                @Override
                public String getBodyContentType() {
                    return "application/json; charset=utf-8";
                }

                @Override
                public byte[] getBody() throws AuthFailureError {
                    try {
                        return requestBody == null ? null : requestBody.getBytes("utf-8");
                    } catch (UnsupportedEncodingException uee){
                        signinError.setText("unsupported encoding");
                        return null;
                    }
                }

                @Override
                protected Response<String> parseNetworkResponse(NetworkResponse response) {
                    String responseString ="";
                    if(response != null){
                        responseString = String.valueOf(response.statusCode);
                    }
                    return Response.success(responseString, HttpHeaderParser.parseCacheHeaders(response));
                }
            };

            stringRequest.setRetryPolicy(new DefaultRetryPolicy(
                    50000,
                    DefaultRetryPolicy.DEFAULT_MAX_RETRIES,
                    DefaultRetryPolicy.DEFAULT_BACKOFF_MULT));
            requestQueue.add(stringRequest);
        } catch (JSONException e){
            e.printStackTrace();
        }

    }
}
