package com.example.tp1.fragments;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import com.example.tp1.R;
import com.example.tp1.adapters.ServicesPostAdapter;
import com.example.tp1.models.ServicesPost;
import com.example.tp1.models.User;
import com.google.gson.Gson;

import java.util.ArrayList;
import java.util.List;

public class ServicesFragment extends Fragment {
    private ListView listServicesPostViews;
    private User currentUser;


    public static ServicesFragment newInstance(){
        return (new ServicesFragment());
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_services, container, false);
        currentUser = new Gson().fromJson(getArguments().getString("user"), User.class);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        listServicesPostViews = (ListView)getView().findViewById(R.id.list_posts_services);
        View headerView = getLayoutInflater().inflate(R.layout.fragment_services_header_list, null);
        listServicesPostViews.addHeaderView(headerView);
        ServicesPostAdapter adapt = new ServicesPostAdapter(load(),getActivity().getBaseContext());
        listServicesPostViews.setAdapter(adapt);
    }


    private List<ServicesPost> load() {
        List<ServicesPost> result = new ArrayList();
        result.add(new ServicesPost("titre1", "content1", "author1"));
        result.add(new ServicesPost("titre2", "content2", "author2"));
        result.add(new ServicesPost("titre3", "content3", "author3"));
        result.add(new ServicesPost("titre4", "content4", "author4"));

        return result;
    }

}
