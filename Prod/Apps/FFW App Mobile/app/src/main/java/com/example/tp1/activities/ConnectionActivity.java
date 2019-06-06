package com.example.tp1.activities;

import android.app.ActivityManager;
import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.RequestFuture;
import com.android.volley.toolbox.Volley;
import com.example.tp1.R;
import com.example.tp1.models.User;
import com.google.gson.Gson;
import com.google.gson.JsonObject;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

public class ConnectionActivity extends AppCompatActivity {
    private Button validation_btn;
    private EditText input_mail;
    private EditText input_mdp;
    private TextView error_message;
    private String currentUser;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connection);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setTitle("Connexion");

        validation_btn = (Button)findViewById(R.id.validation_btn);
        input_mail = (EditText)findViewById(R.id.input_user_mail);
        input_mdp = (EditText)findViewById(R.id.input_user_mdp);
        error_message = (TextView)findViewById(R.id.error_connection_message);

        validation_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (input_mail.getText().length() > 0 && input_mdp.getText().length() >0){
                    validation_btn.setText("Connexion");
                    tryconnect(input_mdp.getText().toString(), input_mail.getText().toString());
                    validation_btn.setText("Se connecter");
                } else {
                    error_message.setText("Tous les champs doivent être remplis");
                }
            }
        });

    }

    private boolean tryconnect(final String password, final String mail){
        final TextView resultText = (TextView)findViewById(R.id.error_connection_message);
        String mJsonURLString = "";
        RequestQueue requestQueue = Volley.newRequestQueue(this);

        try{
            String mailEncode = URLEncoder.encode(mail, "utf-8");
            String passwordEncode = URLEncoder.encode(password, "utf-8");
            mJsonURLString = getResources().getString(R.string.localhost)+"/users/authentication?email="+ mailEncode + "&password=" + passwordEncode;
        } catch (UnsupportedEncodingException e){
            e.printStackTrace();
            return false;
        }
        if (mJsonURLString.length() > 0){
            final Response.Listener<JSONObject> responseListener = new Response.Listener<JSONObject>() {
                @Override
                public void onResponse(JSONObject response) {
                    currentUser = response.toString();
//                    User currentU = new Gson().fromJson(currentUser, User.class);
                    final SharedPreferences prefs = getSharedPreferences("authentication_dico", MODE_PRIVATE);
                    SharedPreferences.Editor edit = prefs.edit();
                    edit.putString("email", input_mail.getText().toString());
                    edit.putString("mdp", input_mdp.getText().toString());
                    edit.putString("user", currentUser);
                    edit.apply();
                    Intent connectionValidateIntent = new Intent(ConnectionActivity.this, DrawerMenuActivity.class);
                    connectionValidateIntent.putExtra("user",currentUser);
                    startActivity(connectionValidateIntent);
                }
            };
            Response.ErrorListener errorListener = new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    resultText.setText("Mot de passe ou email incorrect");
                }
            };
            JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(
                    Request.Method.GET, mJsonURLString, null, responseListener,errorListener
            );
            requestQueue.add(jsonObjectRequest);
        }
        return true;
    }

}
