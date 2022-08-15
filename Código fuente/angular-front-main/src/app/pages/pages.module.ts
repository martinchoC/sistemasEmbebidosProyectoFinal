import { NgModule } from "@angular/core";
import { IndexComponent } from "./index/index.component";
import { PagesComponent } from './pages.component';
import { SharedModule } from '../shared/shared.module';
import { PAGES_ROUTES } from './pages.routes';
import { HttpClientModule } from "@angular/common/http";
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule, ReactiveFormsModule } from "@angular/forms";
import { SetupComponent } from './setup/setup.component';
import { SavesComponent } from './saves/saves.component';
import { TablaComponent } from "../components/tabla/tabla.component";

@NgModule({
    declarations:[
        IndexComponent,
        PagesComponent,
        SetupComponent,
        SavesComponent,
        TablaComponent
    ],
    exports:[
        IndexComponent,
        PagesComponent

    ],
    imports:[
        SharedModule,
        HttpClientModule,
        BrowserModule,
        FormsModule,
        ReactiveFormsModule,
        PAGES_ROUTES
    ]
})

export class PagesModule{}
